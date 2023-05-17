#ifndef ARGS_H
#define ARGS_H

#include <arpa/inet.h>

extern int times;
extern struct in_addr target;
extern char interface[20];
extern unsigned char local_mac[6];
extern struct in_addr local_ip;

int parse_args(int argc, char *argv[]);

#endif