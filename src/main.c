#include "args.h"
#include "arp.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    int e;
    e = parse_args(argc, argv);
    if (e < 0) return -1;
    e = build_request();
    if (e < 0) return -1;
    return 0;
}