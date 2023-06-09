#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define INVALID_SOCKET -1
#define CONNECT_ERROR -1

int main(int argc, char* argv[]) {
    // 利用Socket API 建立一个简易的TCP客户端
    
    // 1 创建一个socket 套接字
    int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // protocal = 0 自动选择协议
    if(sockfd == INVALID_SOCKET) {
        printf("错误,建立Socket失败...\n");
    }
    else {
        printf("建立Socket成功...\n");
    }

    // 2 连接服务器 connect
    struct sockaddr_in _sin = {};
    _sin.sin_family = PF_INET;
    _sin.sin_port = htons(4567);
    _sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret = connect(sockfd, (struct sockaddr*)&_sin, sizeof(struct sockaddr_in));
    if(ret == CONNECT_ERROR) {
        printf("错误,连接服务器失败...\n");
    }
    else {
        printf("连接服务器成功...\n");
    }

    while(1) {
        // 3 用户输入请求命令
        char cmdBuf[128] = {};
        scanf("%s", cmdBuf);

        // 4 处理请求命令
        if(strcmp(cmdBuf, "exit") == 0) {
            printf("收到exit命令， 任务结束。\n");
            break;
        }
        else {
            // 5 向服务端发送请求
            send(sockfd, cmdBuf, strlen(cmdBuf) + 1, 0);
        }

        // 6 接收服务器信息 recv
        char recvBuf[128] = {};
        int nlen = recv(sockfd, recvBuf, 128, 0);
        if(nlen > 0) {
            printf("接收到数据: %s\n",  recvBuf);
        }
    }

    // 7 关闭套接字 close
    close(sockfd);

    printf("已退出。\n");
    getchar();
    return 0;
}