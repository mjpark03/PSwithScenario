#ifndef __SERVER__
#define __SERVER__

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "terminal.h"

const unsigned int BUF_SIZE = 1024;

class Server {
    private:
        int portNum;
        int client;
        int server;
        char buffer[BUF_SIZE];
        struct sockaddr_in server_addr;
        socklen_t size;
        TerminalPrinter *tp;
    public:
        Server(unsigned int port_num, TerminalPrinter *tp);
        bool init();
        void recv_buf();
        void send_buf(const char *buffer, unsigned int length);
        void uninit();
};

#endif
