#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>


typedef enum {
    REGULAR_FILE,  // 普通文件
    DIRECTORY,     // 目录
    SYMBOLIC_LINK, // 软链接
    OTHER          // 其他类型（如设备文件等特殊类型）
} FileType;


FileType getFileType(mode_t mode) {
    if (S_ISREG(mode))
        return REGULAR_FILE;
    else if (S_ISDIR(mode))
        return DIRECTORY;
    else if (S_ISLNK(mode))
        return SYMBOLIC_LINK;
    else
        return OTHER;
}


void getFilePermissions(mode_t mode, char* permissions) {
    permissions[0] = (mode & S_IRUSR)? 'r' : '-';
    permissions[1] = (mode & S_IWUSR)? 'w' : '-';
    permissions[2] = (mode & S_IXUSR)? 'x' : '-';
    permissions[3] = (mode & S_IRGRP)? 'r' : '-';
    permissions[4] = (mode & S_IWGRP)? 'w' : '-';
    permissions[5] = (mode & S_IXGRP)? 'x' : '-';
    permissions[6] = (mode & S_IROTH)? 'r' : '-';
    permissions[7] = (mode & S_IWOTH)? 'w' : '-';
    permissions[8] = (mode & S_IXOTH)? 'x' : '-';
    permissions[9] = '\0';
}


typedef struct FileInfo {
    char fileName[256];  // 文件名，可根据实际情况调整长度
    FileType fileType;   // 文件类型，通过枚举区分
    mode_t permissions;  // 文件权限，使用系统的mode_t类型
    off_t fileSize;      // 文件大小，off_t可处理较大文件大小，单位通常为字节
    time_t lastModified; // 文件最后修改时间，time_t类型用于存储时间戳
}FileInfo;

FileInfo* getname (char *soft,int *count){

    struct stat sb;
    FileInfo *buff = (FileInfo*)malloc(sizeof(FileInfo)*100);
    if(stat(soft,&sb) == -1){
        perror("stat");
        return NULL;        
    }
    if( getFileType(sb.st_mode) ==  DIRECTORY){
        DIR *dir = opendir(soft);
        struct dirent *file;
        while( (file = readdir(dir)) != NULL && file->d_name[0] != '.'){
            strcpy(buff->fileName, file->d_name);
            buff++;
            *count++;
        }
        closedir(dir);
        return buff;
    }
    if( getFileType(sb.st_mode) != DIRECTORY){
        strcpy(buff->fileName,soft);
        buff++;
        *count++;
        return buff;
    }
    
}

void print(FileInfo*buff){

}

int main(int argc,char *argv[])
{
    int i,flag = 1;
    char c;
    char soft[100];
    int count = 0;
    for(i=1;i<argc;i++){
        if(*argv[i]=='/'){
            strcpy(soft,argv[i]);
            flag = 0;
        }
    }
    if( flag ){
        char current_dir[100];
        if(getcwd(current_dir,sizeof(current_dir)) != NULL){
            strcpy(soft,current_dir);
        }
        else{
            perror("getcwd");
        }
    }
    //注意在后续书写函数时应该判断文件类型...
    for(i=1;i<argc;i++){
        if(*argv[i] == '-'){
            while(c = *++argv[i]){
                switch(c){
                    case 'a':
                    //printf("\n");
                    break;
                    case 'l':
                    //
                    break;
                    case 'R':
                    //
                    break;
                    case 't':
                    //
                    break;
                    case 'r':
                    //
                    break;
                    case 'i':
                    //
                    break;
                    case 's':
                    //
                    break;
                    default:
                    printf("cant't find way...");
                    break;
                }
            }
        }
        else 
            break;
    }
    if(i==1){
        //就只是ls
    }
    if(i==2 && *argv[1] == '/'){
        //是指定目录的ls;
    }
    return 0;
}