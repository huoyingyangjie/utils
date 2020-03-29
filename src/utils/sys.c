//
// Created by yangjiegogogo@gmail.com on 10/10/19.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

long get_nr_open(){
    int fd=open("/proc/sys/fs/nr_open", O_RDONLY);
    char buf[256]={0,};
    ssize_t ret;
    if(fd==-1)
        return -1;
    ret=read(fd,buf,sizeof(buf));
    if(ret<0)
        return -1;
    if(ret==sizeof(buf))
    {
        errno=ENOBUFS;
        return -1;
    }
    close(fd);
    buf[ret-1]='\0';
    return strtol(buf,NULL,10);
}

void print_args(int argc, char ** argv){
    int i=0;
    for(i=0;i<argc;++i){
        printf("%s ", argv[i]);
    }
    printf("\n");
}