#ifndef _EASYTCPCLIENT_HPP_
#define _EASYTCPCLIENT_HPP_

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
#include "MessageHeader.hpp"

class EasyTcpClient
{
private:
    SOCKET _sock;
public:
    EasyTcpClient()
    {
        _sock = INVALID_SOCKET;
    }

    // 虚析构函数
    virtual ~EasyTcpClient()
    {
        Close();
    }

    // 创建socket
    void InitSocket()
    {
#ifdef _WIN32
        // 启动Windows socket 2.x环境
        WORD ver = MAKEWORD(2, 2);
        WSADATA dat;
        WSAStartup(ver, &dat);
#endif
        // 1 创建一个socket 套接字
        if(_sock != INVALID_SOCKET)  // 有连接关闭掉
        {
            printf("<Socket=%d>关闭旧连接...\n", _sock);
            Close();            
        }
        _sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // protocal = 0 自动选择协议
        if(_sock == INVALID_SOCKET) 
        {
            printf("错误,建立Socket失败...\n");
        }
        else 
        {
            printf("建立<Socket=%d>成功...\n", _sock);
        }
    }

    // 连接服务器
    int Connect(const char* ip, unsigned short port)
    {
        if(_sock == INVALID_SOCKET)  // 没连接初始化
        {
            InitSocket();            
        }
        // 2 连接服务器 connect
        sockaddr_in _sin = {};
        _sin.sin_family = PF_INET;
        _sin.sin_port = htons(port);
#ifdef _WIN32
        _sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
        _sin.sin_addr.s_addr = inet_addr(ip);
#endif
        int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
        if(ret == SOCKET_ERROR) 
        {
            printf("<Socket=%d>错误,连接服务器<%s:%d>失败...\n", _sock, ip, port);
        }
        else {
            printf("<Socket=%d>连接服务器<%s:%d>成功...\n", _sock, ip, port);
        }
        return ret;
    }

    // 关闭socket
    void Close()
    {
        if(_sock != INVALID_SOCKET) 
        {
#ifdef _WIN32
            closesocket(sockfd);
            // 清除Windows socket环境
            WSACleanup();
#else
            close(_sock);
#endif
            _sock = INVALID_SOCKET;
        }
    }

    // 查询网络消息
    bool OnRun()
    {
        if(isRun())
        {
            // 设置客户端socket 集合
            fd_set fdRead;
            FD_ZERO(&fdRead);
            FD_SET(_sock, &fdRead);
            timeval t = {1, 0}; // 设置超时时间
            int ret = select(_sock + 1, &fdRead, NULL, NULL, &t);
            if(ret < 0) {
                printf("<Socket=%d>select任务结束1\n", _sock);
                Close();
                return false;
            }

            if(FD_ISSET(_sock, &fdRead)) 
            {
                FD_CLR(_sock, &fdRead);

                if(RecvData(_sock) == -1) 
                {
                    printf("<Socket=%d>select任务结束2\n", _sock);
                    Close();
                    return false;
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

    // 缓冲区最小单元大小
#ifndef RECV_BUFF_SIZE
#define RECV_BUFF_SIZE 10240
#endif  

    // 接收缓冲区
    char _szRecv[RECV_BUFF_SIZE] = {};
    // 第二缓冲区 消息缓冲区
    char _szMsgBuf[RECV_BUFF_SIZE * 10] = {};
    // 消息缓冲区的尾部位置
    int _lastPos = 0;

    // 接收数据  处理粘包 拆分包
    int RecvData(SOCKET cSock)
    {
        // 5 接收客户端数据
        int nLen = (int)recv(cSock, _szRecv, RECV_BUFF_SIZE, 0);
        if(nLen <= 0) 
        {
            printf("<Socket=%d>与服务器断开连接，任务结束。\n", cSock);
            return -1;
        }

        // 将收取到的数据拷贝到消息缓冲区
        memcpy(_szMsgBuf + _lastPos, _szRecv, nLen);
        // 消息缓冲区中的数据尾部位置后移
        _lastPos += nLen;
        // 判断消息缓冲区的数据长度是否大于消息头DataHeader长度
        while (_lastPos >= sizeof(DataHeader))  // 解决粘包
        {
            // 此时知道当前消息的长度
            DataHeader* header = (DataHeader*)_szMsgBuf;
            // 判断消息缓冲区的数据长度大于消息长度
            if(_lastPos >= header->dataLength) // 判断是否少包
            {
                // 剩余未处理缓冲区数据的长度
                int nSize = _lastPos - header->dataLength;
                // 处理网络消息
                OnNetMsg(header);
                // 将消息缓冲区剩余未处理数据前移
                memcpy(_szMsgBuf, _szMsgBuf + header->dataLength, nSize);
                // 消息缓冲区的数据尾部位置前移
                _lastPos = nSize;
            }
            else {
                // 消息缓冲区剩余数据不够一条完整消息
                break;
            }
        }

        return 0;
    }

    // 响应网络消息
    virtual void OnNetMsg(DataHeader* header)
    {
        switch(header->cmd) 
        {
            case CMD_LOGIN_RESULT: 
            {
                LoginResult* login = (LoginResult*)header;
                // printf("<Socket=%d>收到服务端消息: CMD_LOGIN_RESULT, 数据长度 : %d\n", _sock, login->dataLength);
            }
            break;
            case CMD_LOGOUT_RESULT:   
            {
                LogoutResult* logout = (LogoutResult*)header;
                // printf("<Socket=%d>收到服务端消息: CMD_LOGOUT_RESULT, 数据长度 : %d\n", _sock, logout->dataLength);
            }
            break;
            case CMD_NEW_USER_JOIN: 
            {
                NewUserJoin* userJoin = (NewUserJoin*)header;
                // printf("<Socket=%d>收到服务端消息: CMD_NEW_USER_JOIN, 数据长度 : %d\n", _sock, userJoin->dataLength);
            }
            case CMD_ERROR: 
            {
                printf("<Socket=%d>收到服务端消息: CMD_ERROR, 数据长度 : %d\n", _sock, header->dataLength);
            }
            break;
            default:
            {
                printf("<Socket=%d>收到未定义消息, 数据长度 : %d\n", _sock, header->dataLength);
            }
        }
    }

    // 发送数据 
    int SendData(DataHeader* header)
    {
        if(isRun() && header)
        {
            return send(_sock, (const char*)header, header->dataLength, 0);
        }
        return SOCKET_ERROR;
    }    
};

#endif