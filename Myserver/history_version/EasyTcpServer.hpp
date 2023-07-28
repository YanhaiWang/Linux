#ifndef _EASYTCPSERVER_HPP_
#define _EASYTCPSERVER_HPP_

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include<windows.h>
    #include<WinSock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include<unistd.h>
    #include<arpa/inet.h>
    #include<string.h>

    #define SOCKET int
    #define INVALID_SOCKET (SOCKET) (~0)
    #define SOCKET_ERROR            (-1)
#endif

#include <stdio.h>
#include <vector>
#include "MessageHeader.hpp"

class EasyTcpServer
{
private:
    SOCKET _sock;
    std::vector<SOCKET> g_clients;  // 存储客户端的socket
    
public:
    EasyTcpServer()
    {
        _sock = INVALID_SOCKET;
    }

    virtual ~EasyTcpServer()
    {
        Close();
    }

    // 初始化Socket
    SOCKET InitSocket()
    {
#ifdef _WIN32
        // 启动Windows socket 2.x环境
        WORD ver = MAKEWORD(2, 2);
        WSADATA dat;
        WSAStartup(ver, &dat);
#endif
        // 利用Socket API 建立一个简易的TCP服务端
        // 1 创建一个socket 套接字
        if(_sock != INVALID_SOCKET)  // 有连接关闭掉
        {
            printf("<Socket=%d>关闭旧连接...\n", (int)_sock);
            Close();            
        } 
        _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // protocal = 0 自动选择协议
        if(_sock == INVALID_SOCKET) 
        {
            printf("错误,建立Socket失败...\n");
        }
        else 
        {
            printf("建立<Socket=%d>成功...\n", (int)_sock);
        }
        return _sock;
    }

    // 绑定端口号
    int Bind(const char* ip, unsigned short port)
    {
        if(_sock == INVALID_SOCKET)  // 无效连接,初始化一下
        {
            InitSocket();
        }
        // 2 bind 绑定用于接受客户端连接的网络端口
        sockaddr_in _sin = {};
        _sin.sin_family = AF_INET; // domain 协议族
        _sin.sin_port = htons(port); // 端口号 大小端转换字节序 host to net unsigned short
#ifdef _WIN32
        if(ip) {
            _sin.sin_addr.S_un.S_addr = inet_addr(ip);
        }
        else {
            _sin.sin_addr.S_un.S_addr = INADDR_ANY;
        }
#else
        if(ip) {
            _sin.sin_addr.s_addr = inet_addr(ip);
        }
        else {
            _sin.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1"); // IP地址
        }
#endif
        int ret = bind(_sock, (sockaddr*)&_sin, sizeof(_sin));
        if(ret == SOCKET_ERROR) {
            printf("错误,绑定网络端口<%d>失败...\n", port);
        }
        else {
            printf("绑定网络端口<%d>成功\n", port);
        }
    }

    // 监听端口号
    int Listen(int n)
    {
        // 3 listen 监听网络端口
        int ret = listen(_sock, n);
        if(ret == SOCKET_ERROR) {
            printf("Socket=<%d>错误,监听端口失败\n", (int)_sock);
        }
        else {
            printf("Socket=<%d>监听网络端口成功\n", (int)_sock);
        }
    }

    // 接受客户端连接
    SOCKET Accept()
    {
        // 4 accept 等待客户端连接
        sockaddr_in clientAddr = {};
        int nAddrLen = sizeof(clientAddr);
        SOCKET _cSockfd = INVALID_SOCKET;
        // char msgBuf[] = "Hello, I am Server.";
#ifdef _WIN32
        _cSockfd = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
#else
        _cSockfd = accept(_sock, (sockaddr*)&clientAddr, (socklen_t*)&nAddrLen); // 返回客户端的sock信息
#endif    
        if(_cSockfd == INVALID_SOCKET) 
        {
            printf("Socket=<%d>错误,接受到无效客户端的SOCKET...\n", (int)_sock);
        }
        else 
        {
            // 新的客户端加入，通知所有的已经存在的socket连接
            NewUserJoin userJoin;
            SendDataToAll(&userJoin);
            g_clients.push_back(_cSockfd);
            printf("Socket=<%d>新客户端加入: socket = %d, IP = %s\n", (int)_sock, (int)_cSockfd, inet_ntoa(clientAddr.sin_addr));
        }    
        return _cSockfd;
    }

    // 关闭Socket 
    void Close() 
    {
        if(_sock != INVALID_SOCKET) 
        {
            // 防止select出现意外退出  需要关闭所有的套接字socket
#ifdef _WIN32
            for(int n = g_clients.size() - 1; n >= 0 ; n--) 
            {
                closesocket(g_clients[n]);
            }
            // 关闭套接字closesocket
            closesocket(_sock);
            // 清除Windows socket环境
            WSACleanup();
#else
            for(int n = g_clients.size() - 1; n >= 0 ; n--) 
            {
                close(g_clients[n]);
            }

            // 6 关闭套接字 close socket
            close(_sock);
#endif
        }
    }

    // 处理网络消息
    bool OnRun()
    {
        if(isRun())
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
            FD_SET(_sock, &fdRead);
            FD_SET(_sock, &fdWrite);
            FD_SET(_sock, &fdExp);

            SOCKET maxSock = _sock;
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
                Close();
                return false;
            }

            // 判断描述符(socket)是否在集合中   是否有可使用的操作
            if(FD_ISSET(_sock, &fdRead)) 
            {
                FD_CLR(_sock, &fdRead);
                Accept();
            }

            for(int n = g_clients.size() - 1; n >= 0 ; n--) {
                if(FD_ISSET(g_clients[n], &fdRead)) 
                {
                    if(RecvData(g_clients[n]) == -1)
                    {
                        auto iter = g_clients.begin() + n; // std::vector<socket>::iterator
                        if(iter != g_clients.end()) {
                            g_clients.erase(iter);
                        }
                    }   
                }
            }
            return true;
        }
        return false;
    }

    // 是否工作中
    bool isRun()   
    {
        return _sock != INVALID_SOCKET;
    }

    // 接收数据 处理粘包 拆分包
    int RecvData(SOCKET _cSockfd)
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

        recv(_cSockfd, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
        OnNetMsg(_cSockfd, header);

        return 0;
    }

    // 响应网络消息
    virtual void OnNetMsg(SOCKET _cSockfd, DataHeader* header)
    {
        // printf("收到命令: %d 数据长度 : %d\n", header.cmd, header.dataLength);
        switch(header->cmd) 
        {
            case CMD_LOGIN: 
            {
                // 注意 由于在读取login之前已经读取了header所以需要偏移header的大小
                Login* login = (Login*)header;
                printf("收到客户端<Socket=%d>请求: CMD_LOGIN, 数据长度 : %d, userName = %s, PassWord = %s\n", _cSockfd, login->dataLength, login->userName, login->PassWord);
                // 忽略判断用户名密码是否正确的过程
                LoginResult ret;
                send(_cSockfd, (char*)&ret, sizeof(LoginResult), 0);    
            }
            break;
            case CMD_LOGOUT:  
            {
                Logout* logout = (Logout*)header;
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
    }

    // 发送指定Socket数据
    int SendData(SOCKET _cSockfd, DataHeader* header)
    {
        if(isRun() && header)
        {
            return send(_cSockfd, (const char*)header, header->dataLength, 0);
        }
        return SOCKET_ERROR;
    }

    // 发送消息到所有客户端
    void SendDataToAll(DataHeader* header)
    {
        if(isRun() && header)
        {
            for(int n = g_clients.size() - 1; n >= 0 ; n--) 
            {
                SendData(g_clients[n], header);
            }
        }
    }
};

#endif