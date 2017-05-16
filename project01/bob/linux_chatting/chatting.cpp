#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <string>

#define TERMINAL_X_SIZE 80
#define TERMINAL_Y_SIZE 24
#define PROMPT_ID "client"

using namespace std;
class TerminalPrinter {
    private:
        string prompt_id;
        unsigned int last_line_number;
        unsigned int input_line_number;
        void set_cursor(int x, int y);
        void clear_line();
    public:
        TerminalPrinter() : prompt_id(PROMPT_ID),
                            last_line_number(1),
                            input_line_number(TERMINAL_Y_SIZE-1) {
            clear();
            listen();
        }
        void clear();
        void listen();
        void print(string buf);
};
void TerminalPrinter::clear() {
    cout << "\033[H\033[J";
}
void TerminalPrinter::clear_line() {
    cout << "\033[2K";
}
void TerminalPrinter::set_cursor(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
    clear_line();
}
void TerminalPrinter::listen() {
    set_cursor(1, input_line_number);
    cout << prompt_id << "('#' will be close) : ";
}
void TerminalPrinter::print(string buf) {
    set_cursor(1, last_line_number);
    cout << prompt_id << " : " << buf << endl;
    if(last_line_number < 23) last_line_number++;
    else cout << endl;
    listen();
}
int main(void)
{
    TerminalPrinter tp;
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &t);


    char ch;
    string buf;

    while(1) {
        ch = cin.get();
        if(ch == '#') return 0;
        if(ch == 127) {
            cout << "\b\b\b    \b\b\b\b";
            if(buf.size() > 0) buf.erase(buf.size()-1);
            continue;
        }
        if(ch == '\n') {
            tp.print(buf);
            buf.clear();
            continue;
        }
        buf.push_back(ch);
    }
    return 0;
}
