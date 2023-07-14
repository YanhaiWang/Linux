#ifndef __SERVER_H__
#define __SERVER_H__

#define INVALID_SOCKET -1
#define BIND_ERROR -1
#define LISTEN_ERROR -1
#define ACCEPT_ERROR -1
#define CONNECT_ERROR -1
typedef int SOCKET;

enum CMD{
    CMD_LOGIN,
    CMD_LOGINOUT,
    CMD_ERROR
};

// 要保证结构体接收和发送的字节序一致
struct DataHeader
{
    short dataLength;
    short cmd;
};

// DataPackage
struct Login
{
    char userName[32];
    char PassWord[32];
};

struct LoginResult{
    int result;
};

struct Logout{
    char userName[32];
};

struct LogoutResult{
    int result;
};

struct DataPackage{
    int age;
    char name;
};

#endif