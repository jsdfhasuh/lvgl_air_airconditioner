#include "lm75.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <signal.h>

void lm75_read(int fd,double *buffer)
{
    int data;
    fcntl(fd,F_SETFL,0);
    printf("begin\n");
    read(fd, &data, sizeof(data));
    printf("data is %02x\n",data);
    *buffer = data * 0.125;
    // 等待一段时间（例如 1 秒）
    //sleep(1); // 1000000 微秒 = 1 秒
    printf("exit\n");
    return;
}