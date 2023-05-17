#include "args.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

struct option opts[] = {
    {"iface", required_argument, 0, 'i'},
    {"addr", required_argument, 0, 'a'},
    {"num", required_argument, 0, 'n'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

char optstr[] = "i:a:n:h";

int times;
struct in_addr target;
char interface[20];
unsigned char local_mac[6];
struct in_addr local_ip;

int copy_string(char *cnf, const char *arg, int sz) {
    int len = strlen(arg);
    // too long file/directoty name
    if (len >= sz) return 1;
    // copy string
    memcpy(cnf, arg, len + 1);
    return 0;
}

int copy_number(int *cnf, const char *arg) {
    char *endptr;
    errno = 0;
    // set 0 to auto choose 10, 8 or 16
    *cnf = strtol(arg, &endptr, 0);
    // something wrong when convert
    if (errno != 0 || *endptr != 0) return 2;
    return 0;
}

void display_help(char *arg0) {
    printf("Usage: %s [-i auto] [-n 3] -a 192.168.43.1 \n", arg0);
    printf(" -i, --iface name  Network interface to get local MAC address\n");
    printf(" -a, --addr IPv4   request IP (numbers-and-dots notation)\n");
    printf(" -n, --num NUM     repeat times\n");
    printf(" -h, --help        display this message and exit\n");
    exit(EXIT_SUCCESS);
}

int parse_args_callback() {
    int e;
    if (times == 0) times = 3;
    printf("Repeat times: %d\n", times);
    if (target.s_addr == 0) {
        fputs(RED "Need target (set it with -a)!\n", stderr);
        return -1;
    }
    printf("Target address: %s\n", inet_ntoa(target));
    if (interface[0] == 0 || strcmp(interface, "auto") == 0) {
        puts(BLUE "Auto select interface..." END);
        e = get_default_interface(interface);
        if (e != 0) return -1;
    }
    printf("Interface: %s\n", interface);
    puts(BLUE "Getting local address..." END);
    e = get_local_addr(interface, local_mac, &local_ip);
    printf("MAC address: %s\n", mac2str(local_mac));
    printf("IP address: %s\n", inet_ntoa(local_ip));
    return 0;
}

int parse_args(int argc, char *argv[]) {
    static char buf[100];
    int c = -1, e = 0;
    puts(BLUE "Parsing options..." END);
    while ((c = getopt_long(argc, argv, optstr, opts, NULL)) != -1) {
        switch (c) {
            case 'h': display_help(argv[0]); break;
            case 'n': {
                e = copy_number(&times, optarg);
                if (e) return fputs(RED "Bad repeat times!\n", stderr), -1;
                break;
            }
            case 'a': {
                e = copy_string(buf, optarg, 99);
                if (e || !inet_aton(buf, &target)) return fputs(RED "Bad address!\n", stderr), -1;
                break;
            }
            case 'i': {
                e = copy_string(interface, optarg, 19);
                if (e) return fputs(RED "Bad interface name!\n", stderr), -1;
                break;
            }
            default: return fputs(RED "Parse args error!\n", stderr), -1;
        }
    }
    return parse_args_callback();
}