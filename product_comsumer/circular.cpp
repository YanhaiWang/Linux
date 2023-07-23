#include "circular_queue.hpp"
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "Task.hpp"
 
using namespace ns_circular_queue;
using namespace ns_task;
 
void *consumer(void *args)
{
    CircularQueue<Task> *cq = (CircularQueue<Task> *)args;
    while (true)
    {
        Task t;
        cq->pop(&t);
        std::cout << "消费数据: " << t.Show()<< "我是线程: " << pthread_self() << std::endl;
        t.run();
        sleep(1);  //1s
    }
}
 
void *producter(void *args)
{
    CircularQueue<Task> *cq = (CircularQueue<Task> *)args;
    const std::string ops = "+-*/%";
    while (true)
    {
        int x = rand() % 20 + 1;
        int y = rand() % 10 + 1;
        char op = ops[rand() % ops.size()];
        Task t(x, y, op);
 
        std::cout << "生产数据:  " << t.Show() << "我是线程: " << pthread_self() << std::endl;
        cq->push(t);
        usleep(100000);  //0.1s
    }
}
 
int main()
{
    srand((long long)time(nullptr));
    CircularQueue<Task> *rq = new CircularQueue<Task>();
 
    pthread_t c1, c2, c3, p1, p2;
 
    pthread_create(&c1, nullptr, consumer, (void *)rq);
    pthread_create(&c2, nullptr, consumer, (void *)rq);
    pthread_create(&c3, nullptr, consumer, (void *)rq);
    pthread_create(&p1, nullptr, producter, (void *)rq);
    pthread_create(&p2, nullptr, producter, (void *)rq);
 
    pthread_join(c1, nullptr);
    pthread_join(c2, nullptr);
    pthread_join(c3, nullptr);
    pthread_join(p1, nullptr);
    pthread_join(p2, nullptr);
 
    return 0;
}