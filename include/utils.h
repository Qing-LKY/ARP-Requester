#ifndef UTILS_H
#define UTILS_H

#include <arpa/inet.h>

#define BLUE "\033[34m"
#define RED "\033[31m"
#define END "\033[0m"

int get_ip_route(char *buf, int mx_siz);
int get_default_interface(char *int_name);

int get_local_mac(int sock, const char *iface, unsigned char *mac);
int get_local_ip(int sock, const char *iface, struct in_addr *ip);
int get_local_addr(const char *iface, unsigned char *mac, struct in_addr *ip);

char *mac2str(unsigned char *mac);

#endif