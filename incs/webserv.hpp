#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#define QUEUE_SIZE 100
#define PORT 3245
#define BUFFER_SIZE 10

int	initServ(void);
int	recv_request(int fd);
