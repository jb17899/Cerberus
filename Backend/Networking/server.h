#ifndef SERVER
#define SERVER

#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<assert.h>
#include<poll.h>
#include<time.h>
#include<unordered_map>
#include<string>
#include "./CommandImp/CommAbs.h"
#include<algorithm>
#include<cstring>
#include <cstdlib>
// #include "./Timers/timer.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"

#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic push
#define MAX_MESSAGE_LEN 4096

#define HOST "127.0.0.1"
#define PORT "12048"




typedef unsigned int int32;
typedef unsigned short int int16;
typedef unsigned char int8;
typedef struct s_client{
    int s;
    char addr[16];
    int16 port;
} Client;

typedef int32 (*Callback)(Client*,int8*,int8*);

typedef struct s_cmdhandler{
    int8* cmd;
    Callback handler;
} CmdHandler;
struct timeVal{
    int32 year;
    int32 month;
    int32 day;
    int32 hour;
    int32 minute;
    int32 sec;
};
int loads(char* buf);
// void mainloop(int);
int initserver(int16_t);
    static std::unordered_map<std::string,HMap>maps;
    static std::unordered_map<std::string,Zset>mapZset;

    int32_t write_full(int fd, char* buf, int n);
#endif
