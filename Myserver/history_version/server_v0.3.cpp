#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include "EasyTcpServer.hpp"

int main(int argc, char* argv[]) 
{    
    EasyTcpServer server;
    server.InitSocket();
    server.Bind(nullptr, 1234);
    server.Listen(5);
    // char _recvBuf[128] = {};

    while(server.isRun()) 
    {
        server.OnRun();
        // printf("空闲时间处理其他业务..\n");
    }
    server.Close();
    printf("已退出。\n");
    getchar();
    return 0;
}