#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
    int fd;
    // 正确打开文件，若文件不存在则创建
    fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open error");
        return -1;
    }

    // 设置文件为非阻塞模式
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("F_GETFL error");
        close(fd);
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("F_SETFL error");
        close(fd);
        return -1;
    }

    int fd1 = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);

    char buffer[MAX_SIZE] = "qwhwhssishqjwlsqwls";
    write(fd, buffer, strlen(buffer));
    printf("%s\n", buffer);

    dup2(STDOUT_FILENO, fd1);
    close(fd);
    close(fd1);

    return 0;
}
