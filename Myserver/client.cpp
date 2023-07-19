#include <thread>
#include "EasyTcpClient.hpp"

void cmdThread(EasyTcpClient* client) 
{
    while(true)
    {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if(strcmp(cmdBuf, "exit") == 0)
        {
            client->Close();
            printf("退出cmdThread线程\n");
            break;
        }
        else if(strcmp(cmdBuf, "login") == 0)
        {
            Login login;
            strcpy(login.userName, "wyh");
            strcpy(login.PassWord,"wyhmm");
            client->SendData(&login);
        }
        else if(strcmp(cmdBuf, "logout") == 0)
        {
            Logout logout;
            strcpy(logout.userName, "wyh");
            client->SendData(&logout);
        }
        else{
            printf("不支持的命令。\n");
        }
    }
}

int main(int argc, char* argv[]) 
{    
    EasyTcpClient client;
    // client.InitSocket();
    client.Connect("127.0.0.1", 4567);
    // 启动线程
    std::thread t1(cmdThread, &client);
    t1.detach(); // 和主线程进程分离

    while(client.isRun()) 
    {
        client.OnRun();
        // printf("空闲时间处理其他业务\n");
        // sleep(1);
    }

    // 7 关闭套接字 close
    client.Close();

    printf("已退出。\n");
    getchar();
    return 0;
}