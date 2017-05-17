#ifndef __TERMINAL__
#define __TERMINAL__

#include "../include/lfu_cache.h"

using namespace std;

typedef struct Cursor {
    unsigned int chat_last_y_pos;
    unsigned int input_y_pos;
    unsigned int input_begin_x_pos;
    unsigned int auto_words_y_pos;
} cursor_t;

class TerminalPrinter {
    private:
        string prompt_id;
        cursor_t *cursor;
        vector<string> auto_words;
        void set_cursor(int x, int y);
        void clear_line();
        LFUCache *lc;
    public:
        TerminalPrinter(const string ID);
        void clear();
        void listen();
        void set_input_mode();
        void print_echo(string buf);
        void print(string buf);
        void show_input_words(char ch);
        void hide_input_words();
        string auto_complete_word(char ch);
};

#endif
