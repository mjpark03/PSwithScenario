#include <iostream>
#include "include/terminal.h"
#include "include/lfu_cache.h"
#include "include/server.h"
using namespace std;

static const char BACKSPACE_INPUT = 127;
static const char END_INPUT = '#';
static const char NEW_LINE = '\n';
static const string BACKSPACE = "\b\b\b    \b\b\b\b";
static const string ID = "server";

int main(void)
{
    TerminalPrinter tp(ID);
    Server sv(9000, &tp);
    if(!sv.init()) {
        cout << "client fail" << endl;
        return 0;
    }

    char ch;
    string buf;

    if(fork() == 0) {
        while(1) {
            sv.recv_buf();
        }
    }
    while(1) {
        ch = cin.get();
        if(ch == END_INPUT) {
            sv.uninit();
            return 0;
        }
        if(ch == BACKSPACE_INPUT) {
            cout << BACKSPACE;
            if(buf.size() > 0) buf.erase(buf.size()-1);
            continue;
        }
        if(ch == NEW_LINE) {
            tp.print(buf);
            buf = ID + " : " + buf;
            sv.send_buf(buf.c_str(), sizeof(buf));
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
