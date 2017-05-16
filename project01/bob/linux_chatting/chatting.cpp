#include <iostream>
#include "include/terminal.h"
#include "include/lfu_cache.h"

using namespace std;

#define BACKSPACE_INPUT 127
#define BACKSPACE "\b\b\b    \b\b\b\b"
#define END_INPUT '#'
#define NEW_LINE '\n'

int main(void)
{
    TerminalPrinter tp;

    static char ch;
    static string buf;

    while(1) {
        ch = cin.get();
        if(ch == END_INPUT) return 0;
        if(ch == BACKSPACE_INPUT) {
            cout << BACKSPACE;
            if(buf.size() > 0) buf.erase(buf.size()-1);
            continue;
        }
        if(ch == NEW_LINE) {
            tp.print(buf);
            buf.clear();
            continue;
        }
        buf.push_back(ch);
    }
    return 0;
}
