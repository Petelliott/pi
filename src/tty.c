#include "tty.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/* writes a null-terminated string to stdout
   unbuffered */
static inline int writestr(const char* str) {
    return write(STDOUT_FILENO, str, strlen(str));
}


// saved by raw_on() and restored by raw_off()
struct termios saved_term;


void raw_on(void) {
    tcgetattr(STDIN_FILENO, &saved_term);
    struct termios raw = saved_term;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


void raw_off(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_term);
}


void alternate_terminal(void) {
    writestr("\e[?1047h");
}


void original_terminal(void) {
    writestr("\e[?1047l");
}


void clear(void) {
    writestr("\e[2J");
}


void init(void) {
    raw_on();
    alternate_terminal();
    clear();
    save_cursor();
    cursor_to(0, 0);
}


void cleanup(void) {
    restore_cursor();
    original_terminal();
    raw_off();
}


void save_cursor(void) {
    writestr("\e[s");
}


void restore_cursor(void) {
    writestr("\e[u");
}


void cursor_to(int x, int y) {
    dprintf(STDOUT_FILENO, "\e[%i;%iH", y+1, x+1);
}


void cursor_up(void) {
    writestr("\e[1A");
}


void cursor_down(void) {
    writestr("\e[1B");
}


void cursor_left(void) {
    writestr("\e[1D");
}


void cursor_right(void) {
    writestr("\e[1C");
}


void cursor_x(int x) {
    dprintf(STDOUT_FILENO, "\e[%iG", x+1);
}


void cursor_y(int y) {
    dprintf(STDOUT_FILENO, "\e[%iD", y+1);
}


int uputchar(int ch) {
    char c = (unsigned char) ch;
    if(write(STDOUT_FILENO, &c, 1) < 0) {
        return EOF;
    } else {
        return ch;
    }
}

int ugetchar() {
    char ch;
    if(read(STDIN_FILENO, &ch, 1) == 1) {
        return ch;
    } else {
        return EOF;
    }
}
