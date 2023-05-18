#ifndef ARP_H
#define ARP_H

#include <linux/if_arp.h>

// ETH_HLEN + sizeof(arp_header)
#define PACARP_LEN 42

struct arp_header {
    __be16 ar_hrd;  /* format of hardware address */
    __be16 ar_pro;  /* format of protocol address */
    unsigned char ar_hln;  /* length of hardware address */
    unsigned char ar_pln;  /* length of protocol address */
    __be16 ar_op;  /* ARP opcode (command) */
    unsigned char ar_sha[ETH_ALEN]; /* sender hardware address */
    unsigned char ar_sip[4];  /* sender IP address  */
    unsigned char ar_tha[ETH_ALEN]; /* target hardware address */
    unsigned char ar_tip[4];  /* target IP address  */
};

int build_request();

#endif