#ifndef __CLIENT__
#define __CLIENT__

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
const unsigned int IP_SIZE = 20;

class Client {
    private:
        int portNum;
        int client;
		char ip[IP_SIZE];
        char buffer[BUF_SIZE];
        struct sockaddr_in client_addr;
        socklen_t size;
        TerminalPrinter *tp;
    public:
        Client(unsigned int port_num, TerminalPrinter *tp);
        bool init();
        void recv_buf();
        void send_buf(const char *buffer, unsigned int length);
        void uninit();
};

#endif
