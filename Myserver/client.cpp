#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server.h"

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
    sockaddr_in _sin = {};
    _sin.sin_family = PF_INET;
    _sin.sin_port = htons(4567);
    _sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret = connect(sockfd, (sockaddr*)&_sin, sizeof(sockaddr_in));
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
            printf("收到exit命令,任务结束。\n");
            break;
        }
        else if(strcmp(cmdBuf, "login") == 0){
            // 5 向服务端发送请求
            Login login;
            strcpy(login.userName, "wyh"); // c++不能对字符数组直接拷贝
            strcpy(login.PassWord, "wyhmima");
            send(sockfd, (const char*)&login, sizeof(login), 0); // 发送消息体
            // 6 接收服务器返回的数据
            LoginResult loginRet = {};
            recv(sockfd, (char*)&loginRet, sizeof(loginRet), 0); // 接收消息体
            printf("LoginResult : %d\n", loginRet.result);
        }
        else if(strcmp(cmdBuf, "logout") == 0){
            // 5 向服务端发送请求
            Logout logout;
            strcpy(logout.userName, "wyh"); 
            send(sockfd, (const char*)&logout, sizeof(logout), 0); // 发送消息体
            // 6 接收服务器返回的数据
            LogoutResult logoutRet = {};
            recv(sockfd, (char*)&logoutRet, sizeof(logoutRet), 0); // 接收消息体
            printf("LogoutResult : %d\n", logoutRet.result);
        }
        else {
            printf("不支持的命令,请重新输入。\n");
        }

        // // 6 接收服务器信息 recv
        // char recvBuf[128] = {};
        // int nlen = recv(sockfd, recvBuf, 128, 0);
        // if(nlen > 0) {
        //     struct DataPackage* dp = (struct DataPackage*) recvBuf;
        //     printf("接收到数据: 年龄=%d , 姓名=%s\n", dp->age, dp->name);
        // }
    }

    // 7 关闭套接字 close
    close(sockfd);

    printf("已退出。\n");
    getchar();
    return 0;
}