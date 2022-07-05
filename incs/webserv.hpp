#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <unistd.h>
#include <poll.h>
#define QUEUE_SIZE 100
#define PORT 3245
#define BUFFER_SIZE 10
#include <sys/select.h>
#include <vector>
#define SEND 100
#define ACCEPT 101
#define RECV 102
#define BUFF_SIZE 10000
