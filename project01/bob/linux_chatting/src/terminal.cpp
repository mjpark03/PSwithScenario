#include "../include/terminal.h"
#include <termios.h>
#include <iostream>

#define TERMINAL_X_SIZE 80
#define TERMINAL_Y_SIZE 24
#define PROMPT_ID "client"

const static unsigned int BEGIN_X_POS = 1;
const static char QUIT_NOTI[] = "('#' will be close) : ";

TerminalPrinter::TerminalPrinter() :
        prompt_id(PROMPT_ID),
        cursor{ BEGIN_X_POS,
                TERMINAL_Y_SIZE-1,
                sizeof(PROMPT_ID) + sizeof(QUIT_NOTI),
                TERMINAL_Y_SIZE } {
    clear();
    listen();
    set_input_mode();
    lc = new LFUCache(5);
}
void TerminalPrinter::clear() {
    cout << "\033[H\033[J";
}
void TerminalPrinter::clear_line() {
    cout << "\033[2K";
}
void TerminalPrinter::set_cursor(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}
void TerminalPrinter::listen() {
    set_cursor(BEGIN_X_POS, cursor.input_y_pos);
    clear_line();
    cout << prompt_id << QUIT_NOTI;
}
void TerminalPrinter::set_input_mode() {
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &t);
}
vector<string> get_string_words(string buf) {
    vector<string> words;
    int word_start_idx = -1;
    size_t i;

    for(i=0;i<buf.size();i++) {
        if(buf[i] == ' ' && word_start_idx != -1) {
            words.push_back(buf.substr(word_start_idx, i - word_start_idx));
            word_start_idx = -1;
        } else if(buf[i] != ' ' && word_start_idx == -1) {
            word_start_idx = i;
        } else if(i == buf.size()-1 && word_start_idx != -1) {
            words.push_back(buf.substr(word_start_idx, i-word_start_idx+1));
        }
    }
    return words;
}
void TerminalPrinter::print(string buf) {
    if(buf.size() == 0) return;

    set_cursor(BEGIN_X_POS, cursor.chat_last_y_pos);
    clear_line();
    cout << prompt_id << " : " << buf << endl;
    if(cursor.chat_last_y_pos < 23) cursor.chat_last_y_pos++;
    else cout << endl;
    listen();

    vector<string> words = get_string_words(buf);
    for(size_t i=0;i<words.size();i++) {
        lc->put(words[i][0], words[i]);
    }
}
void TerminalPrinter::show_input_words(char ch) {
    auto_words = lc->get(ch);
    if(auto_words.size() == 0) return;
    set_cursor(BEGIN_X_POS, cursor.auto_words_y_pos);
    clear_line();
    for(size_t i=0;i<auto_words.size() && i<5;i++) {
        cout << i+1 << "." << auto_words[i] << " ";
    }
    set_cursor(cursor.input_begin_x_pos, cursor.input_y_pos);
}
void TerminalPrinter::hide_input_words() {
    set_cursor(BEGIN_X_POS, cursor.auto_words_y_pos);
    clear_line();
    set_cursor(cursor.input_begin_x_pos+1, cursor.input_y_pos);
    auto_words.clear();
}

string TerminalPrinter::auto_complete_word(char ch) {
    unsigned int word_idx = ch - '1';
    if(auto_words.size() <= word_idx) return NULL;
    set_cursor(BEGIN_X_POS, cursor.auto_words_y_pos);
    clear_line();
    set_cursor(cursor.input_begin_x_pos-1, cursor.input_y_pos);
    cout << auto_words[word_idx];
    return auto_words[word_idx];
}
