#include "../include/terminal.h"
#include <termios.h>

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
void TerminalPrinter::set_input_mode() {
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &t);
}
void TerminalPrinter::print(string &buf) {
    set_cursor(1, last_line_number);
    cout << prompt_id << " : " << buf << endl;
    if(last_line_number < 23) last_line_number++;
    else cout << endl;
    listen();
}

