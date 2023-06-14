#ifndef DGRAM_H
#define DGRAM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORTA 2000
#define MSGSIZE 1000

int create_socket(struct sockaddr_in* saddr,char* ip);

int main_dgram();


#endif

