#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include "server.h"
#include <vector>

std::vector<SOCKET> g_clients;  // 存储客户端的socket

int processor(SOCKET _cSockfd)
{
    // 缓冲区接收数据
    char szRecv[4096] = {}; 
    // 5 接收客户端数据
    int nLen = recv(_cSockfd, szRecv, sizeof(DataHeader), 0);
    DataHeader* header = (DataHeader*)szRecv;
    if(nLen <= 0) {
        printf("客户端<Socket=%d>已退出， 任务结束。\n", _cSockfd);
        return -1;
    }
    // printf("收到命令: %d 数据长度 : %d\n", header.cmd, header.dataLength);
    switch(header->cmd) 
    {
        case CMD_LOGIN: 
        {
            // 注意 由于在读取login之前已经读取了header所以需要偏移header的大小
            recv(_cSockfd, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            Login* login = (Login*)szRecv;
            printf("收到客户端<Socket=%d>请求: CMD_LOGIN, 数据长度 : %d, userName = %s, PassWord = %s\n", _cSockfd, login->dataLength, login->userName, login->PassWord);
            // 忽略判断用户名密码是否正确的过程
            LoginResult ret;
            send(_cSockfd, (char*)&ret, sizeof(LoginResult), 0);    
        }
        break;
        case CMD_LOGOUT: 
        {
            recv(_cSockfd, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            Logout* logout = (Logout*)szRecv;
            printf("收到客户端<Socket=%d>请求: CMD_LOGOUT, 数据长度 : %d, userName = %s\n", _cSockfd, logout->dataLength, logout->userName);
            // 忽略判断用户名密码是否正确的过程
            LogoutResult ret;
            send(_cSockfd, (char*)&ret, sizeof(ret), 0);
        }
        break;
        default:
        {
            DataHeader header = {0, CMD_ERROR};
            send(_cSockfd, (char*)&header, sizeof(header), 0);
        }
        break;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // 利用Socket API 建立一个简易的TCP服务端
    
    // 1 创建一个socket 套接字
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // protocal = 0 自动选择协议
    if(sockfd == INVALID_SOCKET) {
        printf("错误,建立Socket失败...\n");
    }
    else {
        printf("创建Socket成功...\n");
    }

    // 2 bind 绑定用于接受客户端连接的网络端口
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET; // domain 协议族
    _sin.sin_port = htons(4567); // 端口号 大小端转换字节序 host to net unsigned short
#ifdef _WIN32
    _sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
    _sin.sin_addr.s_addr = INADDR_ANY; ; // inet_addr("127.0.0.1"); // IP地址
#endif

    int ret = bind(sockfd, (sockaddr*)&_sin, sizeof(_sin));
    if(ret == SOCKET_ERROR) {
        printf("ERROR,绑定网络端口失败...\n");
    }
    else {
        printf("绑定网络端口成功\n");
    }

    // 3 listen 监听网络端口
    ret = listen(sockfd, 5);

    if(ret == SOCKET_ERROR) {
        printf("ERROR,监听端口失败\n");
    }
    else {
        printf("监听网络端口成功\n");
    }

    // char _recvBuf[128] = {};
    while(1) 
    {
        // 伯克利套接字 BSD socket
        fd_set fdRead; // 描述符(socket)集合
        fd_set fdWrite;
        fd_set fdExp;

        // 清理fd_set集合
        FD_ZERO(&fdRead);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdExp);

        // 将描述符(socket)加入集合
        FD_SET(sockfd, &fdRead);
        FD_SET(sockfd, &fdWrite);
        FD_SET(sockfd, &fdExp);

        SOCKET maxSock = sockfd;
        for(int n = g_clients.size() - 1; n >= 0 ; n--) {
            FD_SET(g_clients[n], &fdRead);
            if(maxSock < g_clients[n])
            {
                maxSock = g_clients[n];
            }
        }

        // nfds 是一个整形值 是指fd_set集合中所有的文件描述符(socket)的范围，而不是数量
        // 是所有文件描述符最大值 + 1  在Windows中这个参数可以写0
        timeval t = {1, 0}; 
        // timeval t = {0, 0} // 设置为非阻塞,处理更多业务
        int ret = select(maxSock + 1, &fdRead, &fdWrite, &fdExp, &t);  // 设置为所有的文件描述符中最大的 + 1
        if(ret < 0) {
            printf("select任务结束。\n");
            break;
        }

        // 判断描述符(socket)是否在集合中   是否有可使用的操作
        if(FD_ISSET(sockfd, &fdRead)) 
        {
            FD_CLR(sockfd, &fdRead);
            // 4 accept 等待客户端连接
            sockaddr_in clientAddr = {};
            int nAddrin = sizeof(clientAddr);
            SOCKET _cSockfd = INVALID_SOCKET;
            // char msgBuf[] = "Hello, I am Server.";
            _cSockfd = accept(sockfd, (sockaddr*)&clientAddr, (socklen_t*)&nAddrin); // 返回客户端的sock信息
            if(_cSockfd == INVALID_SOCKET) {
                printf("错误,接受到无效客户端的SOCKET...\n");
            }
            else 
            {
                // 新的客户端加入，通知所有的已经存在的socket连接
                for(int n = g_clients.size() - 1; n >= 0 ; n--) 
                {
                    NewUserJoin userJoin;
                    send(g_clients[n], (const char*)&userJoin, sizeof(NewUserJoin), 0);
                }
                g_clients.push_back(_cSockfd);
                printf("新客户端加入: socket = %d, IP = %s\n", _cSockfd, inet_ntoa(clientAddr.sin_addr));
            }
        }

        for(int n = g_clients.size() - 1; n >= 0 ; n--) {
            if(FD_ISSET(g_clients[n], &fdRead)) 
            {
                if(processor(g_clients[n]) == -1)
                {
                    auto iter = g_clients.begin() + n; // std::vector<socket>::iterator
                    if(iter != g_clients.end()) {
                        g_clients.erase(iter);
                    }
                }   
            }
        }

        // printf("空闲时间处理其他业务..\n");

        // // 6 处理请求
        // if(strcmp(_recvBuf, "getInfo") == 0) {
        //     // 7 send 向客户端发送一条数据
        //     struct DataPackage dp = {24, "Wang yanhai"};
        //     send(_cSockfd, (const char*)&dp, sizeof(struct DataPackage), 0);
        // }
        // else {
        //     // 7 send 向客户端发送一条数据
        //     char msgBuf[] = "What do you want?";
        //     send(_cSockfd, msgBuf, strlen(msgBuf) + 1, 0); // +1 发送结尾符
        // }
    }
    
    // 防止select出现意外退出  需要关闭所有的套接字socket
#ifdef _WIN32
    for(int n = g_clients.size() - 1; n >= 0 ; n--) 
    {
        closesocket(g_clients[n]);
    }
    // 关闭套接字closesocket
    closesocket(sockfd);
    // 清除Windows socket环境
    WSACleanup();
#else
    for(int n = g_clients.size() - 1; n >= 0 ; n--) 
    {
        close(g_clients[n]);
    }

    // 6 关闭套接字 close socket
    close(sockfd);
#endif

    printf("已退出。\n");
    // getchar();
    return 0;
}