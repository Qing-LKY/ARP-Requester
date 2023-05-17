#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>

int get_ip_route(char *buf, int mx_siz) {
    int e; FILE *fp;
    fp = popen("ip route", "r");
    if (fp == NULL) return perror(RED "popen"), -1;
    e = fread(buf, 1, mx_siz, fp);
    pclose(fp);
    if (e < 0) return perror(RED "fread"), -1;
    return 0;
}

int get_default_interface(char *int_name) {
    static char buf[256], errbuf[256];
    // char *p;
    static const char *pat = "default.*dev\\s+(\\w+)";
    regex_t reg;
    regmatch_t pmat[2];
    int e;
    // Get `ip route`
    e = get_ip_route(buf, sizeof(buf));
    if (e != 0) return -1;
    // match regex
    e = regcomp(&reg, pat, REG_EXTENDED);
    if (e != 0) {
        regerror(e, &reg, errbuf, sizeof(errbuf));
        fprintf(stderr, RED "regcomp: %s\n", errbuf);
        regfree(&reg);
        return -1;
    }
    e = regexec(&reg, buf, 2, pmat, 0);
    if (e == REG_NOMATCH) {
        fprintf(stderr, RED "Default dev not found!");
        regfree(&reg);
        return -1;
    } else if (e != 0) {
        regerror(e, &reg, errbuf, sizeof(errbuf));
        fprintf(stderr, RED "regexec: %s\n", errbuf);
        regfree(&reg);
        return -1;
    }
    sprintf(int_name, "%.*s", pmat[1].rm_eo - pmat[1].rm_so, buf + pmat[1].rm_so);
    // printf("%s %d %d\n", buf, pmat[1].rm_eo, pmat[1].rm_so);
    regfree(&reg);
    return 0;
}

int get_local_mac(int sock, const char *iface, uint8_t *mac) {
    struct ifreq ifr;
    strcpy(ifr.ifr_name, iface);
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) != 0) {
        perror(RED "ioctl(SIOCGIFHWADDR)");
        return -1;
    } 
    memcpy(mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    return 0;
}

int get_local_ip(int sock, const char *iface, struct in_addr *ip) {
    struct ifreq ifr;
    strcpy(ifr.ifr_name, iface);
    if (ioctl(sock, SIOCGIFADDR, &ifr) != 0) {
        perror(RED "ioctl(SIOCGIFADDR)");
        return -1;
    }
    struct sockaddr_in *p = (struct sockaddr_in *)&ifr.ifr_addr;
    *ip = p->sin_addr;
    return 0;
}

int get_local_addr(const char *iface, uint8_t *mac, struct in_addr *ip) {
    int e, s;
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (s == -1) {
        perror(RED "Create socket");
        return -1;
    }
    e = get_local_mac(s, iface, mac);
    if (e != 0) return close(s), -1;
    e = get_local_ip(s, iface, ip);
    return close(s), e ? -1 : 0;
}

char *mac2str(uint8_t *mac) {
    static char buf[100];
    sprintf(buf, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", 
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return buf;
}