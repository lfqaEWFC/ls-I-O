#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[]) {

    int fd = open(argv[1], O_RDWR | O_CREAT, 0754);//最后一个参数是用户的权限
    int fd1 = open(argv[2],O_RDWR | O_CREAT, 0754);

    if (fd == -1) {
        perror("open");
        return -1;
    }//检测error

    off_t lseek_result = lseek(fd1, 0, SEEK_END);
    off_t ler = lseek(fd , 0, SEEK_SET);

    if (lseek_result == -1) {
        perror("lseek");
        close(fd);
        return -1;
    }//检测error

    char buffer[1024];// 创建一个缓冲区

    ssize_t read_result = read(fd, buffer,1024);//将数据读如缓冲区并返回一个读取的字节数

    if (read_result == -1) {
        perror("read");
        close(fd);
        return -1;
    }//检测error

    write(fd1,buffer,read_result);
    printf("%s\n",buffer);
    close(fd);
    close(fd1);

    return 0;
}