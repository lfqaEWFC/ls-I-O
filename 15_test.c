#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc!= 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    struct stat file_stat;

    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return 1;
    }

    // 打印文件类型信息
    printf("File type: ");
    if (S_ISREG(file_stat.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(file_stat.st_mode)) {
        printf("Directory\n");
    } else if (S_ISCHR(file_stat.st_mode)) {
        printf("Character device\n");
    } else if (S_ISBLK(file_stat.st_mode)) {
        printf("Block device\n");
    } else if (S_ISFIFO(file_stat.st_mode)) {
        printf("FIFO (named pipe)\n");
    } else if (S_ISLNK(file_stat.st_mode)) {
        printf("Symbolic link\n");
    } else if (S_ISSOCK(file_stat.st_mode)) {
        printf("Socket\n");
    } else {
        printf("Unknown type\n");
    }

    // 打印文件权限信息
    printf("File permissions: ");
    printf((file_stat.st_mode & S_IRUSR)? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR)? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR)? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP)? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP)? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP)? "x" : "-");
    printf((file_stat.st_mode & S_IROTH)? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH)? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH)? "x" : "-");
    printf("\n");

    // 打印文件的inode号
    printf("Inode number: %lu\n", (unsigned long)file_stat.st_ino);

    // 打印硬连接数目
    printf("Number of hard links: %lu\n", (unsigned long)file_stat.st_nlink);

    // 打印文件所有者ID
    printf("Owner ID: %u\n", file_stat.st_uid);

    // 打印文件所属组ID
    printf("Group ID: %u\n", file_stat.st_gid);

    // 打印设备类型（如果是设备文件）
    if (S_ISCHR(file_stat.st_mode) || S_ISBLK(file_stat.st_mode)) {
        printf("Device type: %lu\n", (unsigned long)file_stat.st_rdev);
    }

    // 打印文件大小（字节）
    printf("File size: %lld bytes\n", (long long)file_stat.st_size);

    // 打印文件系统I/O的块大小
    printf("Block size: %lu bytes\n", (unsigned long)file_stat.st_blksize);

    // 打印文件所占据的块数
    printf("Number of blocks: %lu\n", (unsigned long)file_stat.st_blocks);

    // 打印最后一次访问时间
    printf("Last access time: %s", ctime(&file_stat.st_atime));

    // 打印最后一次修改时间
    printf("Last modification time: %s", ctime(&file_stat.st_mtime));

    // 打印最后一次状态改变时间（指属性）
    printf("Last status change time: %s", ctime(&file_stat.st_ctime));

    return 0;
}
