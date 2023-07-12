#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define INVALID_SOCKET -1
#define BIND_ERROR -1
#define LISTEN_ERROR -1
#define ACCEPT_ERROR -1
// #define SOCKET_ERROR -1
#define SEND_ERROR -1

int main(int argc, char* argv[]) {
    // 利用Socket API 建立一个简易的TCP服务端
    
    // 1 创建一个socket 套接字
    int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // protocal = 0 自动选择协议
    if(sockfd == INVALID_SOCKET) {
        printf("错误,建立Socket失败...\n");
    }
    else {
        printf("创建Socket成功...\n");
    }

    // 2 bind 绑定用于接受客户端连接的网络端口
    struct sockaddr_in _sin = {};
    _sin.sin_family = PF_INET; // domain 协议族

    _sin.sin_port = htons(4567); // 端口号 大小端转换字节序 host to net unsigned short
    
    _sin.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1"); // IP地址

    int ret = bind(sockfd, (struct sockaddr*)&_sin, sizeof(_sin));
    
    if(ret == BIND_ERROR) {
        printf("ERROR,绑定网络端口失败...\n");
    }
    else {
        printf("绑定网络端口成功\n");
    }

    // 3 listen 监听网络端口
    ret = listen(sockfd, 5);

    if(ret == LISTEN_ERROR) {
        printf("ERROR,监听端口失败\n");
    }
    else {
        printf("监听网络端口成功\n");
    }

    // 4 accept 等待客户端连接
    struct sockaddr_in clientAddr = {};
    int nAddrin = sizeof(clientAddr);
    char msgBuf[] = "Hello, I am Server.";

    while(1) {
        int _cSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &nAddrin); // 返回客户端的sock信息

        if(_cSockfd == ACCEPT_ERROR) {
            printf("ERROR,接受到无效客户端的SOCKET...\n");
        }
        printf("新客户端加入: IP = %s\n", inet_ntoa(clientAddr.sin_addr));

        // 5 send 向客户端发送一条数据
        ret = send(_cSockfd, msgBuf, strlen(msgBuf) + 1, 0); // +1 发送结尾符
        if(ret == SEND_ERROR) {
            printf("发送失败...\n");
        }
    }
    
    // 6 关闭套接字 close socket
    close(sockfd);

    return 0;
}