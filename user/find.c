#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#define MAXPATH 512

char* fmtname(char *path){
    static char buf[DIRSIZ+1];
    char* p;

    //Find filename after '/'
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return filename or folder
    if(strlen(p) >= DIRSIZ){
        return p;
    }

    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0;
    return buf;
}

void find(char* path, char* filename){
    char buf[MAXPATH];
    int fd;
    struct dirent de;
    struct stat st;

    //Open folder
    if((fd = open(path, O_RDONLY)) < 0){
        printf("file: can not open %s\n", path);
        close(fd);
        return;
    }

    // Get info file
    if(fstat(fd, &st) < 0){
        printf("find: can not open %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_DEVICE:

    case T_FILE:
        //Compare name file with 'filename'
        if(strcmp(de.name, filename) == 0){
            printf("%s\n", buf);
        }
        break;

    case T_DIR:
        //Check file length
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            close(fd);
            return;
        }

        //Open file
        if((fd= open(path, 0)) < 0){
            printf("find: can not open %s\n", path);
            return;
        }

        // Read folder in folder
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0){
                continue;
            }

            //Skip "." and ".."
            if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
                continue;
            }

            //Re-initialize the path
            memmove(buf, path, strlen(path));
            buf[strlen(path)] = '/';
            memmove(buf + strlen(path) + 1, de.name, strlen(de.name));
            buf[strlen(path) + 1 + strlen(de.name)] = '\0';

            //Read file information
            if(stat(buf, &st) < 0){
                printf("find: can not stat %s\n", buf);
               continue;
            }

            //Compare name file with 'filename'
            if(st.type == T_FILE && strcmp(de.name, filename) == 0){
                printf("%s\n", buf);
            }

            //Recursive if file is folder
            if(st.type == T_DIR){
                find(buf, filename);
            }
        }
    }
    close(fd);
}

int main(int argc, char* argv[]){
    //Check the number of parameters
    if(argc < 3){
        printf("Usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}