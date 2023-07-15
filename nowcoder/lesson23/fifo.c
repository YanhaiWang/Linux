/*
    #include <sys/types.h>
    #include <sys/stat.h>
    int mkfifo(const char *pathname, mode_t mode);
    参数：
        -pathname : 管道名称的路径
        -mode : 文件的权限 和 open 的mode 是一样的
    返回值：成功返回0，失败返回-1和错误号

*/  

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    // 判断文件是否存在
    int ret = access("fifo1", F_OK);
    if(ret == -1) {
        printf("管道不存在，创建管道");

        ret = mkfifo("fifo1", 0664);
    
        if(ret == -1) {
            perror("fifo");
            exit(0);
        }
    }

    return 0;
}
    
