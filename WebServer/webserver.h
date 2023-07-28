#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>


#include "./http/http_conn.h"

const int MAX_FD = 65536; // 最大文件描述符数
const int MAX_EVENT_NUMBER = 10000; // 最大事件数
const int TIMESLOT = 5; // 最小超时时间

class WebServer
{
public:
    WebServer();
    ~WebServer();

    void init(int port, string user, string passwd, string dataBaseName,
              int log_write, int opt_write, int trigmode, int sql_num,
              int thread_num, int close_log, int actor_model);

    void thread_pool();
    void sql_pool();
    void log_write();
    void trig_mode();
    void eventListen();
    void timer(int connfd, struct sockaddr_in client_address);
    void adjust_timer(util_timer *timer);
    void deal_timer(util_timer *timer, int sockfd);
    bool dealclientdata();
    bool dealwithsignal(bool& timout, bool& stop_server);
    void dealwiththread(int sockfd);
    void dealwithwrite(int sockfd);

public:
    // 基础信息
    int port; // 端口
    char* m_root; // 根目录
    int m_log_write; // 日志类型
    int m_close_log; // 是否启动日志
    int m_actormodel; // IO模式  Reactor/Proactor

    // 网络参数
    int m_pipefd[2]; // 连接的套接字
    int m_epollfd; // epoll对象
    http_conn *users; // 单个http连接

    // 数据库相关
    connection_pool *m_connPool;
    string m_user; // 登录数据库的用户名
    string m_passwd; // 数据库登录密码
    string m_dataBaseName; // 数据库名
    int m_sql_num; // 数据库连接池数量

    // 线程池相关
    thread_pool<http_conn> *m_pool;
    int m_thread_num; // 线程数

    // epoll_event 相关
    epoll_event events[MAX_EVENT_NUMBER];

    int m_listenfd; // 监听套接字
    int m_OPT_LINGER; // 是否完美下线
    int m_TRIGMode; // 触发模式 ET/LT
    int m_LISTENTrigmode; // 监听触发模式 ET/LT
    int m_CONNTrigmode; // 连接触发模式  ET/LT

    // 定时器相关
    client_data *users_timer;

    // 工具类
    Utils utils;
};

#endif