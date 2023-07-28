#pragma once

// mprpc框架的基础类  用一个单例实现
class MprpcApplication
{
public:
    static void Init(int argc, char **argv);
    static MprpcApplication& GetInstance();
private:
    MprpcApplication() {}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(const MprpcApplication&&) = delete;
};

