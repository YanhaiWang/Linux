#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <iostream>
#include <pthread.h>

namespace ns_task
{
    class Task
    {
    private:
        int _x;
        int _y;
        char _op; // 操作符
    public:
        Task() {}
        Task(int x, int y, char op) : _x(x), _y(y), _op(op)
        {
        }
        std::string Show()
        {
            std::string message = std::to_string(_x);
            message += _op;
            message += std::to_string(_y);
            message += "=?";
            return message;
        }
        int run()
        {
            int res = 0;
            switch (_op)
            {
            case '+':
                res = _x + _y;
                break;
            case '-':
                res = _x - _y;
                break;
            case '*':
                res = _x * _y;
                break;
            case '/':
                res = _x / _y;
                break;
            case '%':
                res = _x % _y;
                break;
            default:
                std::cout << "something wrong" << std::endl;
                break;
            }
            std::cout << "当前任务正在被线程: " << pthread_self() << " 处理: "
                      << _x << _op << _y << "=" << res << std::endl;
            return res;
        }
        ~Task() {}
    };
}

#endif