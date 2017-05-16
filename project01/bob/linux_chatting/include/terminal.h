#ifndef __TERMINAL__
#define __TERMINAL__

#include <iostream>
#include <stdio.h>

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
            set_input_mode();
        }
        void clear();
        void listen();
        void set_input_mode();
        void print(string &buf);
};

#endif
