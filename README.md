# ARP-Requester

## 题目要求

1. 阅读课本第10、11章。
2. 了解数据链路层数据的获取方法，包括设置套接口以捕获链路帧的编程方法、从套接口读取链路帧的编程方法、定位IP包头的编程方法、定位TCP报头的编程方法、定位UDP报头的编程方法和定位应用层报文数据的编程方法；
3. 使用SOCK_PACKET编写ARP请求程序：
  了解ARP协议；
  使用发送ARP请求数据
  使用ARP命令查看ARP表并验证；

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
