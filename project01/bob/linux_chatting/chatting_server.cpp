#include <iostream>
#include "include/terminal.h"
#include "include/lfu_cache.h"
#include "include/server.h"
using namespace std;

static const char BACKSPACE_INPUT = 127;
static const char END_INPUT = '#';
static const char NEW_LINE = '\n';
static const string BACKSPACE = "\b\b\b    \b\b\b\b";

int main(void)
{
    TerminalPrinter tp;
    Server sv(9000);
    if(!sv.init()) cout << "server fail" << endl;

    char ch;
    string buf;

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
        if(ch >= '1' && ch <= '5') {
            buf = tp.auto_complete_word(ch);
            continue;
        }
        buf.push_back(ch);
        if(buf.size() == 1) {
            tp.show_input_words(buf[0]);
        } else if(buf.size() == 2) {
            tp.hide_input_words();
        }
    }
    return 0;
}
