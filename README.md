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
sudo ./arp-send -i enth0 -n 3 -a 192.168.43.1
```

不指定 `-i` 或指定为 auto，将自动获取默认路由接口。

发送的包可以用下面的指令截到:

```bash
sudo tcpdump arp
```

该方式可以检查是否发送成功以及是否有 Response 返回。

## Note

### 1

默认路由接口是通过指令 `ip route` 的返回结果判断的。

### 2

课本上使用的是已经弃用的 `SOCK_PACKET` 写法。此处改进为新的写法。

### 3

参考 `man packet`，if_ether.h 里的宏定义需要经过 htons 的转换。理由是:

> protocol is the IEEE 802.3 protocol number in network byte order

### 4

头文件 <linux/if_arp.h> 中的 arphdr 不能直接用，它是不完整的，我们要自己定义一个。理由是: 

> 部分内容的大小收协议和硬件影响，不能写死在结构体里。

### 5

`__bitwise__` 的含义是大端。但它本身不会对 gcc 编译产生影响，只是一种提醒作用。

对于 `be16` 类型的变量，记得 `htons` 即可。
