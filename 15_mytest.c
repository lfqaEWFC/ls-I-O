#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

struct stat sb;

int main(int argc,char *argv[]){
    
    if(argc<2){
        printf("File name is %s\n",argv[0]); 
    }

    if(stat(argv[1],&sb)==-1){
        printf("struct stat creat error....\n");
    }
    // 打印文件类型信息


    // 打印文件类型信息
    printf("文件的类型是: ");
    if (S_ISREG(sb.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(sb.st_mode)) {
        printf("Directory\n");
    } else if (S_ISCHR(sb.st_mode)) {
        printf("Character device\n");
    } else if (S_ISBLK(sb.st_mode)) {
        printf("Block device\n");
    } else if (S_ISFIFO(sb.st_mode)) {
        printf("FIFO (named pipe)\n");
    } else if (S_ISLNK(sb.st_mode)) {
        printf("Symbolic link\n");
    } else if (S_ISSOCK(sb.st_mode)) {
        printf("Socket\n");
    } else {
        printf("未知类型\n");
    }

    printf("该文件的权限是:");
    printf((sb.st_mode & S_IRUSR)? "r" : "-");
    printf((sb.st_mode & S_IWUSR)? "w" : "-");
    printf((sb.st_mode & S_IXUSR)? "x" : "-");
    printf((sb.st_mode & S_IRGRP)? "r" : "-");
    printf((sb.st_mode & S_IWGRP)? "w" : "-");
    printf((sb.st_mode & S_IXGRP)? "x" : "-");
    printf((sb.st_mode & S_IROTH)? "r" : "-");
    printf((sb.st_mode & S_IWOTH)? "w" : "-");
    printf((sb.st_mode & S_IXOTH)? "x" : "-");
    printf("\n");

    printf("The file's size is %lu bytes\n",(long)sb.st_size);
    printf("%s",ctime(&sb.st_atime));

}