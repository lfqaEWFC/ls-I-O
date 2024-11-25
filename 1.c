#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[]) {

    int fd = open(argv[1], O_RDWR | O_CREAT, 0754);
    int fd1 = open(argv[2],O_RDWR | O_CREAT, 0754);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    off_t lseek_result = lseek(fd1, 0, SEEK_END);
    off_t ler = lseek(fd , 0, SEEK_SET);
    if (lseek_result == -1) {
        perror("lseek");
        close(fd);
        return -1;
    }
    char buffer[1024];
    ssize_t read_result = read(fd, buffer,1024);
    if (read_result == -1) {
        perror("read");
        close(fd);
        return -1;
    }

    write(fd1,buffer,read_result);
    printf("%s\n",buffer);
    close(fd);
    close(fd1);

    return 0;
}
/*#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc!= 3) {
        printf("用法: %s 源文件 目标文件\n", argv[0]);
        return -1;
    }

    // 打开源文件，以只读方式打开
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open源文件");
        return -1;
    }

    // 打开目标文件，以读写方式打开，若不存在则创建，权限设置为0666
    int fd1 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd1 == -1) {
        perror("open目标文件");
        close(fd);
        return -1;
    }

    char buffer[1024];
    ssize_t read_bytes;
    // 循环读取源文件并写入目标文件，直到读取完整个源文件
    while ((read_bytes = read(fd, buffer, sizeof(buffer))) > 0) {
        ssize_t written_bytes = write(fd1, buffer, read_bytes);
        if (written_bytes == -1) {
            perror("write");
            close(fd);
            close(fd1);
            return -1;
        }
    }

    if (read_bytes == -1) {
        perror("read");
        close(fd);
        close(fd1);
        return -1;
    }

    close(fd);
    close(fd1);
    return 0;
}
*/
  