# ARP-Requester

## 题目要求

## Usage

```text
Usage: ./arp-send [-i auto] [-n 3] -a 192.168.43.1 
 -i, --iface name  Network interface to get local MAC address
 -a, --addr IPv4   request IP (numbers-and-dots notation)
 -n, --num NUM     repeat times
 -h, --help        display this message and exit
```

例如，以 enth0 为网络接口发送对 192.168.43.1 的 arp 请求，请求重复 3 次:

```bash
./arp-send -i enth0 -n 3 -a 192.168.43.1
```

不指定 `-i` 或指定为 auto，将自动获取默认路由接口。

## Note

默认路由接口是通过指令 `ip route` 的返回结果判断的。
