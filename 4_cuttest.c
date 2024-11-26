#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define max_size 1024
int main(int argc,char *argv[]){
    int fd = open(argv[1],O_RDWR | O_CREAT, 0644);
    int cpfd = open(argv[2] ,O_RDWR | O_CREAT, 0644);
    char buffer[max_size];
    size_t readsize = read (fd ,buffer ,max_size);
    printf("%s",buffer);
    off_t lseeknum = lseek(cpfd ,0 ,SEEK_END);
    write(cpfd, buffer ,readsize);
}