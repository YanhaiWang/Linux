/*
    #include <unistd.h>
    int execl(const char *pathname, const char *arg, ...);
        -参数：
            -path：需要指定的执行的文件的路径或者名称
                a.out /home/xxxx/a.out  推荐使用绝对路径 

            -arg：是可执行文件所需要的参数列表
                第一个参数一般没什么作用，为了方便，一般写的是执行程序的名称
                从第二个参数开始往后，就是程序执行所需要的参数列表。
                参数最后需要以NULL结束（哨兵）
        
        - 返回值
            只有调用失败，才有返回值，返回-1，并且设置errno
            如果调用成功，没有返回值
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    // 创建一个子进程，在子进程中执行exec函数族中的函数
    __pid_t pid = fork(); 

    if(pid > 0) {
        // 父进程
        printf("i am parent process, pid : %d\n", getpid());
        sleep(1);
    }
    else if(pid == 0){
        // 子进程
        // execl("hello", "hello", NULL);
        // execl("/bin/ps", "ps", "aux", NULL);
        execl("ps", "ps", "aux", NULL); // 不写路径找不到ps
        perror("execl");

        printf("i am child process, pid : %d\n", getpid());

    }

    for(int i = 0; i < 3; i++) {
        printf("i = %d, pid = %d\n", i, getpid());
    }

    return 0;
}