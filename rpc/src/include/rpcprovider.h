#pragma once
#include "google/protobuf/service.h"

// 框架提供的专门发布rpc服务的网络对象类
class RpcProvider
{
public:
    // 这个框架是提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    // 启动rpc节点，开始提供rpc远程网络调用服务
    void Run();
};