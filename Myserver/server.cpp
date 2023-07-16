#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include "server.h"
#include <vector>
// #include <thread>

struct NewUserJoin : public DataHeader
{
    NewUserJoin() {
        dataLength = sizeof(NewUserJoin);
        cmd = CMD_NEW_USER_JOIN;
        sock = 0;
    }
    int sock;
};

std::vector<SOCKET> g_clients;  

int processor(SOCKET _cSockfd)
{
    // 缓冲区接收数据
    char szRecv[1024] = {}; 
    // 接收客户端数据
    int nLen = recv(_cSockfd, szRecv, sizeof(DataHeader), 0);
    DataHeader* header = (DataHeader*)szRecv;
    if(nLen <= 0) {
        printf("客户端已退出， 任务结束。\n");
        return -1;
    }

    switch(header->cmd) {
        case CMD_LOGIN: 
        {
            // 注意 由于在读取login之前已经读取了header所以需要偏移header的大小
            recv(_cSockfd, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            Login* login = (Login*)szRecv;
            printf("收到命令: CMD_LOGIN, 数据长度 : %d, userName = %s, PassWord = %s\n", login->dataLength, login->userName, login->PassWord);
            // 忽略判断用户名密码是否正确的过程
            LoginResult ret;
            send(_cSockfd, (char*)&header, sizeof(DataHeader), 0);
            send(_cSockfd, (char*)&ret, sizeof(LoginResult), 0);    
        }
        break;
        case CMD_LOGOUT: 
        {
            recv(_cSockfd, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
            Logout* logout = (Logout*)szRecv;
            printf("收到命令: CMD_LOGOUT, 数据长度 : %d, userName = %s\n", logout->dataLength, logout->userName);
            // 忽略判断用户名密码是否正确的过程
            LogoutResult ret;
            send(_cSockfd, (char*)&header, sizeof(header), 0);
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
#ifdef _WIN32
    // 启动windows socket 2.x环境
    WORD ver = MAKEWORD(2, 2);
    WSADARA dat;
    WSAStartup(ver, &dat);
#endif
    // 利用Socket API 建立一个简易的TCP服务端
    
    // 1 创建一个socket 套接字
    SOCKET sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // protocal = 0 自动选择协议
    if(sockfd == INVALID_SOCKET) {
        printf("错误,建立Socket失败...\n");
    }
    else {
        printf("创建Socket成功...\n");
    }

    // 2 bind 绑定用于接受客户端连接的网络端口
    sockaddr_in _sin = {};
    _sin.sin_family = PF_INET; // domain 协议族
    _sin.sin_port = htons(4567); // 端口号 大小端转换字节序 host to net unsigned short

#ifdef _WIN32    
    _sin.sin_addr.S_un.S_addr = INADDR_ANY; // inet_addr("127.0.0.1"); // IP地址
#else    
    _sin.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1"); // IP地址
#endif

    int ret = bind(sockfd, (sockaddr*)&_sin, sizeof(_sin));
    
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

    while(1) 
    {
        // 伯克利 socket
        fd_set fdRead;
        fd_set fdWrite;
        fd_set fdExp;
        
        // 清空 fd_set集合
        FD_ZERO(&fdRead);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdExp);

        // 将服务端文件描述符（socket）加入集合
        FD_SET(sockfd, &fdRead);
        FD_SET(sockfd, &fdWrite);
        FD_SET(sockfd, &fdExp);
        // SOCKET maxSock = sockfd;

        // 将加入的客户端加入到查询集合中  在可读文件描述符中查询看是否有数据要接收
        for(int n = (int)g_clients.size() - 1; n >= 0; n--)
        {
            FD_SET(g_clients[n], &fdRead);
            // if(maxSock < g_clients[n]) {
            //     maxSock = g_clients[n];
            // }
        }

        // nfds 是一个整形值 是指fd_set集合中所有的文件描述符(socket)的范围，而不是数量
        // 是所有文件描述符最大值 + 1  在Windows中这个参数可以写0
        timeval t = {0, 0};
        int ret = select(sockfd + 1, &fdRead, &fdWrite, &fdExp, &t);
        if(ret < 0) {
            printf("select任务结束。\n");
            break;
        }

        // 判断描述符（socket）是否在集合中
        if(FD_ISSET(sockfd, &fdRead)) {
            FD_CLR(sockfd, &fdRead); // 清理fdRead
            // 4 accept 等待客户端连接
            sockaddr_in clientAddr = {};
            int nAddrlen = sizeof(clientAddr);
            SOCKET _cSockfd = INVALID_SOCKET;
            // char msgBuf[] = "Hello, I am Server.";

            _cSockfd = accept(sockfd, (sockaddr*)&clientAddr, (socklen_t*)&nAddrlen); // 返回客户端的sock信息
            if(_cSockfd == INVALID_SOCKET) {
                printf("ERROR,接受到无效客户端的SOCKET...\n");
            }
            // else{
                // for(int n = (int)g_clients.size() - 1; n >= 0; n--) {
                //     NewUserJoin userJoin;
                //     send(g_clients[n], (const char*)&userJoin, sizeof(NewUserJoin), 0);
                // }
                g_clients.push_back(_cSockfd); 
                printf("新客户端加入: socket = %d, IP = %s\n", _cSockfd, inet_ntoa(clientAddr.sin_addr));
            // }
        }

#ifdef _WIN32
        for(size_t n = 0; n < fdRead.fd_count; n++)
        {
            if(processor(fdRead.fd_array[n]) == -1)
            {
                auto iter = std::find(g_clients.begin(), g_clients.end(), fdRead.fd_array[n]);
                if(iter != g_clients.end()) {
                    g_clients.erase(iter);
                }
            }
        }
#else
        for(int n = (int)g_clients.size() - 1; n >= 0; n--)
        {
            if(FD_ISSET(g_clients[n], &fdRead)) {
                if(processor(g_clients[n]) == -1)
                {
                    auto iter = g_clients.begin(); // std::vector<socket>::iterator
                    if(iter != g_clients.end()) {
                        g_clients.erase(iter);
                    }
                }
            }
        }
#endif
        
        // printf("收到命令: %d 数据长度 : %d\n", header.cmd, header.dataLength);
    }

#ifdef _WIN32
    // 如果有异常 将所有的socket全部关闭
    for(size_t n = g_clients.size() - 1; n >= 0; n--)
    {
        closesocket(g_clients[n]);
    }
    
    // 6 关闭套接字 close socket
    closesocket(sockfd);
    //--------------
    // 清除Windows socket环境
    WSACleanup();
#else
    // 如果有异常 将所有的socket全部关闭
    for(int n = (int)g_clients.size() - 1; n >= 0; n--)
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