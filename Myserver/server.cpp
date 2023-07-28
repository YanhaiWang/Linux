#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include "EasyTcpServer.hpp"
#include <thread>

bool g_bRun = true;
void cmdThread() 
{
    while(true)
    {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if(strcmp(cmdBuf, "exit") == 0)
        {
            g_bRun = false;
            printf("退出cmdThread线程\n");
            break;
        }
        else{
            printf("不支持的命令。\n");
        }
    }
}

int main(int argc, char* argv[]) 
{    
    EasyTcpServer server;
    server.InitSocket();
    server.Bind(nullptr, 1234);
    server.Listen(5);
    // char _recvBuf[128] = {};

    // 启动线程
    std::thread t1(cmdThread);
    t1.detach(); // 和主线程进程分离

    while(g_bRun) 
    {
        server.OnRun();
        // printf("空闲时间处理其他业务..\n");
    }
    server.Close();
    printf("已退出。\n");
    getchar();
    return 0;
}