/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    int open(const char *pathname, int flags, mode_t mode);
        参数：
            -pathname：要打开的文件路径
            -flags：对文件的操作权限设置和其他设置
                -必选项：O_RDONLY, O_WRONLY, ORDWR 这三个权限是互斥的
                -可选项：O_CREAT 文件不存在，创造新文件
            -mode：八进制的数，表示创建出的新的文件的操作权限，比如：0775
            最终的权限是：mode & ~umask  root umask:0022 usr umask:0002
            rwx -> 111  -rwxrwxrwx  777 用户权限 当前组权限 其他组权限 
            0777   ->   111111111 
        &   0755   ->   111101101
        --------------------------
                        111101101  -> 0755
            umask的作用就是抹去某些权限

            flags参数是一个int类型的数据，占4个字节，32位
            flags 32位，每一个就是一个标志位

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // 创建一个新的文件
    int fd = open("create.txt", O_RDWR | O_CREAT, 0777);

    if(fd == -1) {
        perror("open");
    }

    // 关闭
    close(fd);

    return 0;
}