#ifndef _CIRCULAR_QUEUE_HPP
#define _CIRCULAR_QUEUE_HPP

#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
 
namespace ns_circular_queue
{
    const int default_len = 10;
 
    template <class T>
    class CircularQueue
    {
    private:
        std::vector<T> _circular_queue;
        int _len;
        // 生产者关心空位置资源
        sem_t _empty_sem;
        // 消费者关心数据资源
        sem_t _data_sem;
 
        int _c_pos;
        int _p_pos;
 
        pthread_mutex_t _c_mtx;
        pthread_mutex_t _p_mtx;
 
    public:
        CircularQueue(int len = default_len) : _circular_queue(len), _len(len)
        {
            sem_init(&_empty_sem, 0, len);
            sem_init(&_data_sem, 0, 0);
            _c_pos = _p_pos = 0;
 
            pthread_mutex_init(&_c_mtx, nullptr);
            pthread_mutex_init(&_p_mtx, nullptr);
        }
        ~CircularQueue()
        {
            sem_destroy(&_empty_sem);
            sem_destroy(&_data_sem);
 
            pthread_mutex_destroy(&_c_mtx);
            pthread_mutex_destroy(&_p_mtx);
        }
 
    public:
        void push(const T &in)
        {
            // 生产接口
            sem_wait(&_empty_sem); // P(empty)，尝试申请empty资源，如果没有empty资源（信号值 <= 0)，当前线程会被挂起
            pthread_mutex_lock(&_p_mtx);
 
            _circular_queue[_p_pos] = in; // 对于生产者，直接在_p_pos处放入生产的数据即可，当前_p_pos处是没有data的
            _p_pos++;
            _p_pos %= _len;

            pthread_mutex_unlock(&_p_mtx);
            sem_post(&_data_sem); // V(data)，释放data资源，如果信号值>0，其他正在调用sem_wait等待信号量的线程将被唤醒
        }
        void pop(T *out)
        {
            // 消费接口
            sem_wait(&_data_sem); // P(data)
            pthread_mutex_lock(&_c_mtx);

            *out = _circular_queue[_c_pos];
            _c_pos++;
            _c_pos %= _len;

            pthread_mutex_unlock(&_c_mtx);
            sem_post(&_empty_sem); // V(empty)
        }
    };
}

#endif