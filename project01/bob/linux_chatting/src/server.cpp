#include "../include/server.h"

using namespace std;
Server::Server(unsigned int port_num, TerminalPrinter *tp) : portNum(port_num), tp(tp) {
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    size = sizeof(server_addr);
}

bool Server::init() {
    client = socket(AF_INET, SOCK_STREAM, 0);

    int option = 1;
    setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option) );

    if (client < 0) {
        tp->print_echo("Error establishing socket...");
        return false;
    }

    tp->print_echo("=> Socket server has been created...");
    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) {
        tp->print_echo("=> Error binding connection, the socket has already been established...");
        return false;
    }

    tp->print_echo("=> Looking for clients...");

    listen(client, 1);

    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    if (server < 0) {
        tp->print_echo("=> Error on accepting...");
        return false;
    }

    tp->print_echo("=> Connected..");
    return true;
}

void Server::recv_buf() {
    recv(server, buffer, BUF_SIZE, 0);
    tp->print_echo(string(buffer));
}

void Server::send_buf(const char *buffer, unsigned int length) {
    send(server, buffer, BUF_SIZE, 0);
}

void Server::uninit() {
    cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
    close(server);
    cout << "\nGoodbye..." << endl;
    exit(1);
    close(client);
}