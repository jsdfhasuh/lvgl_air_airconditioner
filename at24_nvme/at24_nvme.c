
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "at24.h"





void print_buffer(const char *buffer,size_t length)
{
    printf("eeprom buffer is \n");
    for (size_t i = 0;i < length;i++)
    {
        printf("%02x",buffer[i]);
    }
    printf("\n");
}

int read_eeprom(size_t offset,size_t length,void *buffer,int fd)
{
    
    if (pread(fd, buffer,length,offset) != length)
    {
        printf("error read EEPROM");
        return -1;
    }
    
}

int write_eeprom(size_t offset,size_t length,void *buffer,int fd)
{
    if (pwrite(fd, buffer, length, offset) != length)
    {
        printf("error write EEPROM");
        return -1;
    }
}

