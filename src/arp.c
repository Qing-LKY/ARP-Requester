#include "arp.h"
#include "args.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>

uint8_t bcast_mac[ETH_ALEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void build_packet(char *ef) {
    struct ethhdr *p_ethhdr;
    struct arp_header *p_arphdr;
    // set ether header
    p_ethhdr = (struct ethhdr *)ef;
    memcpy(p_ethhdr->h_source, local_mac, ETH_ALEN);
    memcpy(p_ethhdr->h_dest, bcast_mac, ETH_ALEN);
    p_ethhdr->h_proto = htons(ETH_P_ARP);
    // set arp header
    p_arphdr = (struct arp_header *)(ef + ETH_HLEN);
    p_arphdr->ar_hrd = htons(ARPHRD_ETHER);
    p_arphdr->ar_pro = htons(ETH_P_IP); // ipv4
    p_arphdr->ar_hln = ETH_ALEN;
    p_arphdr->ar_pln = sizeof(struct in_addr);
    p_arphdr->ar_op = htons(ARPOP_REQUEST);
    memcpy(p_arphdr->ar_sha, local_mac, ETH_ALEN);
    memcpy(p_arphdr->ar_sip, &local_ip, sizeof(struct in_addr));
    memcpy(p_arphdr->ar_tha, bcast_mac, ETH_ALEN);
    memcpy(p_arphdr->ar_tip, &target, sizeof(struct in_addr));
}

void init_sockaddr(struct sockaddr_ll *dev) {
    memset(dev, 0, sizeof(struct sockaddr_ll));
    dev->sll_family = AF_PACKET; /* Always AF_PACKET */
    dev->sll_protocol = htons(ETH_P_ARP); /* Physical-layer protocol */
    dev->sll_ifindex = ifindex; /* Interface number */
    dev->sll_hatype = ARPHRD_ETHER; /* ARP hardware type */
    dev->sll_pkttype = PACKET_BROADCAST;  /* Packet type */
    dev->sll_halen = ETH_ALEN; /* Length of address */
    memcpy(dev->sll_addr, local_mac, ETH_ALEN); /* Physical-layer address */
}

int build_request() {
    static char ef[PACARP_LEN];
    int i, e, s;
    struct sockaddr_ll dev;
    s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (s < 0) {
        perror(RED "Init socket");
        return -1;
    }
    puts(BLUE "Building ARP-Request packet..." END);
    build_packet(ef);
    init_sockaddr(&dev);
    for (i = 0; i < times; i++) {
        e = sendto(s, ef, PACARP_LEN, 0, (struct sockaddr *)&dev, sizeof(dev));
        if (e < 0) {
            perror(RED "Send packet");
            close(s);
            return -1;
        }
        printf(BLUE "Sending #%d...\n" END, i);
        sleep(1);
    }
    return close(s), 0;
}