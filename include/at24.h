#ifndef __AT24_H__
#define __AT24_H__
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int read_eeprom(size_t offset,size_t length,void *buffer,int fd); 
int write_eeprom(size_t offset,size_t length,void *buffer,int fd);
void print_buffer(const char *buffer,size_t length);

#endif