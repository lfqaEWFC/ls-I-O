#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

struct optionflag{
    int optiona;
    int optioni;
    int optionl;
    int options;
    int optionr;
    int optiont;
    int optionR;
}optionflag;

typedef enum {
    REGULAR_FILE,
    DIRECTORY,     
    SYMBOLIC_LINK,
    GREEN_EXECUTABLE,
    OTHER
} FileType;


FileType getFileType(mode_t mode) {
 
    if (S_ISDIR(mode)) {
        return DIRECTORY;
    }
    else if (S_ISLNK(mode)) {
        return SYMBOLIC_LINK;
    }
    else if (S_ISREG(mode)) {
        if ((mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH)) {
            return GREEN_EXECUTABLE;
    }
    else {
        return REGULAR_FILE;
    }
}
    if (S_ISREG(mode)) {
        return REGULAR_FILE;
    }
    return OTHER;
}

void getFilePermissions(mode_t mode, char* permissions) {
    permissions[0] = (S_ISDIR(mode)) ? 'd' : '-';
    permissions[1] = (mode & S_IRUSR)? 'r' : '-';
    permissions[2] = (mode & S_IWUSR)? 'w' : '-';
    permissions[3] = (mode & S_IXUSR)? 'x' : '-';
    permissions[4] = (mode & S_IRGRP)? 'r' : '-';
    permissions[5] = (mode & S_IWGRP)? 'w' : '-';
    permissions[6] = (mode & S_IXGRP)? 'x' : '-';
    permissions[7] = (mode & S_IROTH)? 'r' : '-';
    permissions[8] = (mode & S_IWOTH)? 'w' : '-';
    permissions[9] = (mode & S_IXOTH)? 'x' : '-';
    permissions[10] = '\0';
}


typedef struct FileInfo {
    char fileName[256];     // 文件名，可根据实际情况调整长度
    FileType fileType;      // 文件类型，通过枚举区分
    char permissions[10];   // 文件权限，使用系统的mode_t类型
    off_t fileSize;         // 文件大小，off_t可处理较大文件大小，单位通常为字节
    time_t lastModified;    // 文件最后修改时间，time_t类型用于存储时间戳
    blkcnt_t blockSize;     // 文件的内存块数；
    nlink_t linknumber;     // 文件的硬链接数
    uid_t uid;              // 文件所有者
    gid_t gid;              // 文件所属组
    ino_t inode;            // 文件inode号
}FileInfo;

struct winsize getTerminalSize() {
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1) {
        size.ws_row = 24;//计算行数
        size.ws_col = 80;//计算列数(保证后续打印的对齐输出)
    }
    return size;
}

FileInfo* getmessage (char *soft,int *count,struct optionflag optionflag){
    *count = 0;
    struct stat sb;
    char full_path[1024];
    int sz = strlen(soft);
    FileInfo *buff = (FileInfo*)malloc(sizeof(FileInfo)*60000);
    if(lstat(soft,&sb) == -1){
        perror("lstat");
        return NULL;        
    }
    if(access(soft,R_OK) == -1){
        perror("无权限读取该目录");
        return NULL;         
    }   
    if( getFileType(sb.st_mode) ==  DIRECTORY){
        DIR *dir = opendir(soft);
        struct dirent *file;
        while( (file = readdir(dir)) != NULL){
            if(file->d_name[0] == '.' && !optionflag.optiona){
                continue;
            }
            else{
                snprintf(full_path, sizeof(full_path), "%s/%s", soft, file->d_name);
                if (lstat(full_path, &sb) == -1) {
                perror("lstat for sub file");
                continue;
            }
                strcpy(buff[*count].fileName, file->d_name);
                buff[*count].fileType = getFileType(sb.st_mode);
                getFilePermissions(sb.st_mode, buff[*count].permissions);
                buff[*count].fileSize = sb.st_size;
                buff[*count].lastModified = sb.st_mtime;
                buff[*count].linknumber = sb.st_nlink;
                buff[*count].uid = sb.st_uid;
                buff[*count].gid = sb.st_gid;
                buff[*count].blockSize = sb.st_blocks / 2;
                buff[*count].inode = sb.st_ino;
                (*count)++;}
        }
        closedir(dir);
        return buff;
    }
    if( getFileType(sb.st_mode) != DIRECTORY){
        strcpy(buff[*count].fileName,soft);
        buff[*count].fileType = getFileType(sb.st_mode);
        (*count)++;
        return buff;
    }
    
}

void print(FileInfo* buff, int count,struct optionflag optionflag,struct winsize size) {
    blkcnt_t sumblock = 0;
    unsigned short sumcol; 
    int maxInodeWidth = 0;
    int maxblockwitch = 0;
    int maxfilewitch = 0;
    int maxlinkwitch = 0;
    int maxsizewitch = 0;
    if(optionflag.optioni||optionflag.optionl||optionflag.options){
        for(int j = 0;j < count; j++){
            if(optionflag.optioni){
                char maxinodeStr[64];  
                sprintf(maxinodeStr, "%lu", (unsigned long)buff[j].inode);
                int currentInodeWidth = strlen(maxinodeStr);
                if (currentInodeWidth > maxInodeWidth) {
                    maxInodeWidth = currentInodeWidth;
                }
            }
            if(optionflag.options){
                char maxblockstr[20];
                sprintf(maxblockstr, "%lu", (unsigned long)buff[j].blockSize);
                int currentblockwitch = strlen(maxblockstr);
                if(currentblockwitch > maxblockwitch) {
                    maxblockwitch = currentblockwitch;
                }
            }        
            sumblock = buff[j].blockSize + sumblock;
        }
    }
    for(int k = 0;k<count;k++){
        int currentfilewitch = strlen(buff[k].fileName);
        if(currentfilewitch > maxfilewitch) {
            maxfilewitch = currentfilewitch;
        }         
    }    
    if(optionflag.options||optionflag.optionl){        
        printf("总计 %lu\n",(unsigned long)sumblock);        
    }
    int width =  maxblockwitch + maxfilewitch + maxInodeWidth + 1;
    if(optionflag.optioni)
        width++;
    if(optionflag.options)
        width++;
    int number = size.ws_col/width;
    if (optionflag.optionl){
        for(int j=0;j<count;j++){
            char maxlinkstr[20];
            sprintf(maxlinkstr, "%lu", (unsigned long)buff[j].linknumber);
            int currentlinkwitch = strlen(maxlinkstr);
            if(currentlinkwitch > maxlinkwitch) {
                maxlinkwitch = currentlinkwitch;
            }
            char maxsizestr[20];
            sprintf(maxsizestr, "%lu", (unsigned long)buff[j].fileSize);
            int currentsizewitch = strlen(maxsizestr);
            if(currentsizewitch > maxsizewitch) {
                maxsizewitch = currentsizewitch;
            }
        }    
    }
    for (int i = 0; i < count; i++) {
        if(i!=0 && i%number == 0 && !optionflag.optionl){
            printf("\n");
        }
        if(optionflag.optioni){
            char inodeStr[64];  
            sprintf(inodeStr, "%lu", (unsigned long)buff[i].inode);
            printf("%*s ",maxInodeWidth,inodeStr);
        }
        if(optionflag.options){
            char blockstr[20];
            sprintf(blockstr, "%lu", (unsigned long)buff[i].blockSize);
            printf("%*s ",maxblockwitch, blockstr);
        }
        if(optionflag.optionl){
            struct passwd *pw = getpwuid(buff[i].uid);
            struct group *gr = getgrgid(buff[i].gid);
            char timeStr[20];
            struct tm *timeInfo = localtime(&buff[i].lastModified);
            strftime(timeStr, sizeof(timeStr), "%m月 %d %H:%M", timeInfo);
            char linkstr[20];
            sprintf(linkstr, "%lu", (unsigned long)buff[i].linknumber);
            char sizestr[20];
            sprintf(sizestr, "%lld", (long long)buff[i].fileSize);                        
            switch (buff[i].fileType) {
                case DIRECTORY:
                    printf("%s %*s %s %s %*s %s \033[0;34m%s\033[0m\n",buff[i].permissions,maxlinkwitch,linkstr,pw->pw_name,gr->gr_name,maxsizewitch,sizestr,timeStr,buff[i].fileName);
                    break; 
                case SYMBOLIC_LINK:
                    printf("%s %*s %s %s %*s %s \033[0;36m%s\033[0m\n",buff[i].permissions,maxlinkwitch,linkstr,pw->pw_name,gr->gr_name,maxsizewitch,sizestr,timeStr,buff[i].fileName);
                    break;
                case GREEN_EXECUTABLE:
                    printf("%s %*s %s %s %*s %s \033[0;32m%s\033[0m\n",buff[i].permissions,maxlinkwitch,linkstr,pw->pw_name,gr->gr_name,maxsizewitch,sizestr,timeStr,buff[i].fileName);
                    break;
                default:
                    printf("%s %*s %s %s %*s %s %s\n",buff[i].permissions,maxlinkwitch,linkstr,pw->pw_name,gr->gr_name,maxsizewitch,sizestr,timeStr,buff[i].fileName);
                    break;
            }
            continue;
        }
        switch (buff[i].fileType) {
            case DIRECTORY:
                printf("\033[0;34m%-*s \033[0m", maxfilewitch, buff[i].fileName);
                break; 
            case SYMBOLIC_LINK:
                printf("\033[0;36m%-*s \033[0m", maxfilewitch, buff[i].fileName);
                break;
            case GREEN_EXECUTABLE:
                printf("\033[0;32m%-*s \033[0m", maxfilewitch, buff[i].fileName);
                break;
            default:
                printf("%-*s ", maxfilewitch, buff[i].fileName);
                break;
        }
    }
    if(!optionflag.optionl)
        printf("\n");

    //free(buff);

    maxInodeWidth = 0;
    maxblockwitch = 0;
    maxfilewitch = 0;
    maxlinkwitch = 0;
    maxsizewitch = 0;

}

void swap(FileInfo *a, FileInfo *b) {

    FileInfo temp = *a;
    *a = *b;
    *b = temp;

}

int timecmp(FileInfo *a, FileInfo *b) {
    // 使用标准库函数difftime来比较时间戳大小，它返回的是double类型的差值
    double diff = difftime(b->lastModified, a->lastModified);
    if (diff > 0) {
        return 1;
    } else if (diff < 0) {
        return -1;
    }
    // 如果时间戳相等，再按照文件名进行字典序比较
    return strcmp(a->fileName, b->fileName);
}


int filecmp(FileInfo *a,FileInfo *b){
    char *p = a->fileName;
    char *q = b->fileName;
    if(strcmp(a->fileName,"..") == 0 && strcmp(b->fileName , ".") == 0){
        return 1;
    }
    while(*p != '\0'||*q != '\0'){
        if(*p==*q){
            p++;
            q++;
            continue;
        }
        if(*p=='.'){
            p++;
            continue;
        }
        if(*q=='.'){
            q++;
            continue;
        }
        if(*p>*q){
            return 1;
        }
        if(*p<*q){
            return 0;
        }

    }
    return 0;    
}

void sort(FileInfo*buff,int count,struct optionflag optionflag){
    
    if (optionflag.optiont) {
        // 使用冒泡排序按照时间比较来交换元素顺序
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (optionflag.optionr && timecmp(&buff[j + 1], &buff[j]) > 0) {
                    swap(&buff[j], &buff[j + 1]);
                    continue;
                }
                if (timecmp(&buff[j], &buff[j + 1]) > 0 &&!optionflag.optionr) {
                    swap(&buff[j], &buff[j + 1]);
                }
            }
        }
        return;
    }
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if(optionflag.optionr && filecmp(&buff[j+1],&buff[j])){
                swap(&buff[j],&buff[j+1]);
                continue;
            }
            if(filecmp(&buff[j],&buff[j+1]) && !optionflag.optionr){
                swap(&buff[j],&buff[j+1]);
            }
        }
    }

}

int listR(char *soft,int *count,struct optionflag optionflag){
    printf("%s:\n",soft);
    FileInfo *buff=getmessage(soft,count,optionflag);
    if(buff == NULL){
        return -1;
    }
    sort(buff,*count,optionflag);
    struct winsize size = getTerminalSize();
    print(buff,*count,optionflag,size);
    int counter = *count;
    if(counter)
        printf("\n");
    for(int k=0;k<counter;k++){
        if(buff[k].fileType == DIRECTORY){
            char sub_path[1024];
        if( !strcmp(buff[k].fileName,".") || !strcmp(buff[k].fileName,"..")){
            continue;
        }
            snprintf(sub_path, sizeof(sub_path), "%s/%s", soft, buff[k].fileName);
            if(listR(sub_path, count, optionflag) == -1){
                continue;
            }
        }
    }

}

int main(int argc,char *argv[])
{
    optionflag.optiona = 0;
    optionflag.optioni = 0;
    optionflag.optionl = 0;
    optionflag.optionr = 0;
    optionflag.options = 0;
    optionflag.optiont = 0;

    int i,flag = 1,k=0;
    char c;
    char soft[10][100];
    int count = 0;
    for(i=1;i<argc;i++){
        if(*argv[i]=='/'){
            strcpy(soft[k],argv[i]);
            k++;
            flag = 0;
        }
    }
    if( flag ){
        char current_dir[100];
        if(getcwd(current_dir,sizeof(current_dir)) != NULL){
            strcpy(soft[k],current_dir);
        }
        else{
            perror("getcwd");
        }
    }

    for(i=1;i<argc;i++){
        if(*argv[i] == '-'){
            while(c = *++argv[i]){
                switch(c){
                    case 'a':
                        optionflag.optiona = 1;
                    break;
                    case 'l':
                        optionflag.optionl = 1;
                    break;
                    case 'R':
                        optionflag.optionR = 1;
                    break;
                    case 't':
                        optionflag.optiont = 1;
                    break;
                    case 'r':
                        optionflag.optionr = 1;
                    break;
                    case 'i':
                        optionflag.optioni = 1;
                    break;
                    case 's':
                        optionflag.options = 1;
                    break;
                    default:
                    printf("cant't find way...");
                    break;
                }
            }
        }
        else 
            continue;
    }

    if(k>1){
        for(int m=0;m<k;m++){
            if(optionflag.optionR){
                listR(soft[m],&count,optionflag);
                continue;
            }
            printf("%s:\n",soft[m]);
            FileInfo *buff=getmessage(soft[m],&count,optionflag);
            sort(buff,count,optionflag);
            struct winsize size = getTerminalSize();
            print(buff,count,optionflag,size);
            if(m != k-1)
                printf("\n");
            }
        }
    else{
        if(optionflag.optionR){
            listR(soft[0],&count,optionflag);
            return 0;
        }
        FileInfo *buff=getmessage(soft[0],&count,optionflag);
        sort(buff,count,optionflag);
        struct winsize size = getTerminalSize();
        print(buff,count,optionflag,size);
    }

    return 0;

}
/*
    写旗子:
    对旗子的结构体可以分为两大类
    (1）改变文件的打印方式的旗子
        i,l,s
    (2) 改变排序方式的旗子
        r,t;
    (3)将各类旗子写入两大函数中判断并封装；
    (4)最后在main函数中写R来实现高层逻辑递归；（目前的思路是写一个递归的函数，这个函数囊括了已经架构好框架的get和print函数）

    12.9 计划完成输出模块；（已完成）
    12.9 完成后续递归思路和框架搭建；
    12.15 1.计划完成分类模块(12.15)；2.封装一个list函数方便后续递归；3.并改写函数支持输入多个目录；；
    12.15-12.16 完成递归模块(完成了75%)。
    12.16 1.解决a和R不兼容问题；2.sort函数在对.和..排序的时候的乱序问题

    关于递归：
    在main函数中创建一个函数;
    在函数中录入所有的目录文件到一个新缓冲区；

*/