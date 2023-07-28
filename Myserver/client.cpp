#include <thread>
#include "EasyTcpClient.hpp"

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
    const int cCount = 10;
    EasyTcpClient* client[cCount];
    // client.InitSocket();
    for(int n = 0; n < cCount; n++)
    {
        if(!g_bRun) 
        {
            return 0; 
        }
        client[n] = new EasyTcpClient();
    }

    for(int n = 0; n < cCount; n++)
    {
        if(!g_bRun) 
        {
            return 0; 
        }
        client[n]->Connect("127.0.0.1", 1234); // 104.168.96.123
    }
    
    // 启动线程
    std::thread t1(cmdThread);
    t1.detach(); // 和主线程进程分离

    Login login;
    strcpy(login.userName, "wyh");
    strcpy(login.PassWord, "wyhmm");

    while(g_bRun) 
    {
        for(int n = 0; n < cCount; n++)
        {
            client[n]->SendData(&login);
            // client[n]->OnRun();
        }
        // printf("空闲时间处理其他业务\n");
        // sleep(1);
    }
 
    // 7 关闭套接字 close
    for(int n = 0; n < cCount; n++)
    {
        client[n]->Close();
    }

    printf("已退出。\n");
    // getchar();
    return 0;
}