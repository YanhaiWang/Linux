/*
    #include <sys/types.h>
    #include <unistd.h>

    pid_t fork(void);
        函数的作用：用于创建子进程。
        返回值：
            fork()的返回值会返回两次。一次是在父进程中，一次是在子进程中。
            在父进程中返回创建的子进程的ID,
            在子进程中返回0
            如何区分父进程和子进程：通过fork的返回值
            在父进程中返回-1，表示创建子进程失败，并设置errno  当进程数到达系统上限会返回EAGAIN 当系统内存不足会返回ENOMEN
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {

    int num = 10;
    // 创建子进程
    pid_t pid = fork();

    // 判断是父进程还是子进程
    if(pid > 0) {
        printf("pid : %d\n", pid);
        // 如果是大于0，返回的是创建的子进程编号，当前是父进程
        printf("i am parent process, pid : %d, ppid : %d\n", getpid(), getppid());
        printf("parent num : %d\n", num);
        num += 10;
        printf("parent num += 10 : %d\n", num);
    }
    else if(pid == 0) {
        // 当前是子进程
        printf("i am child process, pid : %d, ppid : %d\n", getpid(), getppid());
        printf("child num : %d\n", num);
        num += 100;
        printf("child num += 100 : %d\n", num);
    }

    // for循环
    for(int i = 0; i < 3; i++) {
        printf("i : %d , pid : %d\n", i, getpid());
        sleep(1);
    }

    return 0;
}