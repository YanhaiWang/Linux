#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <thread>
#include "server.h"

int processor(SOCKET _cSockfd)
{
    // 缓冲区接收数据
    char szRecv[4096] = {}; 
    // 5 接收客户端数据
    int nLen = recv(_cSockfd, szRecv, sizeof(DataHeader), 0);
    DataHeader* header = (DataHeader*)szRecv;
    if(nLen <= 0) {
        printf("与服务器断开连接，任务结束。\n");
        return -1;
    }
    // printf("收到命令: %d 数据长度 : %d\n", header.cmd, header.dataLength);
    switch(header->cmd) 
    {
        case CMD_LOGIN_RESULT: 
        {
            recv(_cSockfd, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            LoginResult* login = (LoginResult*)szRecv;
            printf("收到服务端消息: CMD_LOGIN_RESULT, 数据长度 : %d\n", login->dataLength);
        }
        break;
        case CMD_LOGOUT_RESULT: 
        {
            recv(_cSockfd, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            LogoutResult* logout = (LogoutResult*)szRecv;
            printf("收到服务端消息: CMD_LOGOUT_RESULT, 数据长度 : %d\n", logout->dataLength);
        }
        break;
        case CMD_NEW_USER_JOIN: 
        {
            recv(_cSockfd, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            NewUserJoin* userJoin = (NewUserJoin*)szRecv;
            printf("收到服务端消息: CMD_NEW_USER_JOIN, 数据长度 : %d\n", userJoin->dataLength);
        }
        break;
    }
    return 0;
}

bool g_bRun = true;

void cmdThread(SOCKET sock) 
{
    while(g_bRun)
    {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if(strcmp(cmdBuf, "exit") == 0)
        {
            g_bRun = false;
            printf("退出cmdThread线程\n");
            break;
        }
        else if(strcmp(cmdBuf, "login") == 0)
        {
            Login login;
            strcpy(login.userName, "wyh");
            strcpy(login.PassWord,"wyhmm");
            send(sock, (const char*)&login, sizeof(Login), 0);
        }
        else if(strcmp(cmdBuf, "logout") == 0)
        {
            Logout logout;
            strcpy(logout.userName, "wyh");
            send(sock, (const char*)&logout, sizeof(Logout), 0);
        }
        else{
            printf("不支持的命令。\n");
        }
    }
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    // 启动Windows socket 2.x环境
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);
#endif
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
#ifdef _WIN32
    _sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
    _sin.sin_addr.s_addr = inet_addr("172.17.0.13");
#endif
    int ret = connect(sockfd, (sockaddr*)&_sin, sizeof(sockaddr_in));
    if(ret == SOCKET_ERROR) {
        printf("错误,连接服务器失败...\n");
    }
    else {
        printf("连接服务器成功...\n");
    }

    // 启动线程函数
    std::thread t1(cmdThread, sockfd);
    t1.detach(); // 和主线程进程分离

    while(g_bRun) {
        // 设置客户端socket 集合
        fd_set fdRead;
        FD_ZERO(&fdRead);
        FD_SET(sockfd, &fdRead);
        timeval t = {1, 0}; // 设置超时时间
        int ret = select(sockfd + 1, &fdRead, NULL, NULL, &t);
        if(ret < 0) {
            printf("select任务结束1\n");
            break;
        }

        if(FD_ISSET(sockfd, &fdRead)) {
            FD_CLR(sockfd, &fdRead);

            if(processor(sockfd) == -1) {
                printf("select任务结束2\n");
            break;
            }
        }

        // printf("空闲时间处理其他业务\n");
        
        // sleep(1);
    }

    // 7 关闭套接字 close
#ifdef _WIN32
    closesocket(sockfd);
    // 清除Windows socket环境
    WSACleanup();
#else
    close(sockfd);
#endif
    printf("已退出。\n");
    getchar();
    return 0;
}