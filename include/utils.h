#ifndef UTILS_H
#define UTILS_H

#include <arpa/inet.h>

#define BLUE "\033[34m"
#define RED "\033[31m"
#define END "\033[0m"

int get_ip_route(char *buf, int mx_siz);
int get_default_interface(char *int_name);

int get_local_mac(int sock, const char *iface, uint8_t *mac);
int get_local_ip(int sock, const char *iface, struct in_addr *ip);
int get_iface_index(int sock, const char *iface, int *ind);
int get_local_addr(const char *iface, uint8_t *mac, struct in_addr *ip, int *ind);

char *mac2str(uint8_t *mac);

#endif