#include "../include/client.h"

using namespace std;

Client::Client(unsigned int port_num) : portNum(port_num) {
    strcpy(ip, "127.0.0.1");

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(portNum);
    inet_pton(AF_INET, ip, &client_addr.sin_addr);

    size = sizeof(client_addr);

    tp = new TerminalPrinter();
}

bool Client::init() {
    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
    {
        tp->print_echo("Error establishing socket...");
        return false;
    }

    tp->print_echo("=> Socket client has been created...");

    if (connect(client,(struct sockaddr *)&client_addr, sizeof(client_addr)) == 0)
        tp->print_echo("=> Connection to the server port number: " + portNum);
    else return false;

    tp->print_echo("=> Connected...");
    return true;
}

void Client::recv_buf() {
    recv(client, buffer, BUF_SIZE, 0);
    tp->print_echo(string(buffer));
}

void Client::send_buf(char *buffer, unsigned int length) {
    send(client, buffer, BUF_SIZE, 0);
}

void Client::uninit() {
    cout << "\n=> Connection terminated.\nGoodbye...\n";
    close(client);
}