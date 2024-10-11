#ifndef INFRARED_H
#define INFRARED_H

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
int infrared_read_raw_data(char * buffer,int fd);
void print_raw_data(char * buffer);
int open_port(char *com,void (*sig_func)(int sig));
#endif