#define _GNU_SOURCE
#include "tty.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdarg.h>


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
    twritestr("\e[?1047h");
}


void original_terminal(void) {
    twritestr("\e[?1047l");
}


void clear(void) {
    twritestr("\e[2J");
}


int t_width;
int t_height;

static void refresh_term_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    t_width = w.ws_col;
    t_height = w.ws_row;
}


void init(void) {
    raw_on();
    alternate_terminal();
    clear();
    save_cursor();
    cursor_to(0, 0);

    refresh_term_size();
    refresh();
}


void cleanup(void) {
    restore_cursor();
    original_terminal();
    raw_off();
    refresh();
}


void save_cursor(void) {
    twritestr("\e[s");
}


void restore_cursor(void) {
    twritestr("\e[u");
}


void cursor_to(int x, int y) {
    tprintf("\e[%i;%iH", y+1, x+1);
}


void cursor_up(void) {
    twritestr("\e[1A");
}


void cursor_down(void) {
    twritestr("\e[1B");
}


void cursor_left(void) {
    twritestr("\e[1D");
}


void cursor_right(void) {
    twritestr("\e[1C");
}


void cursor_x(int x) {
    tprintf("\e[%iG", x+1);
}


void cursor_y(int y) {
    tprintf("\e[%iD", y+1);
}


int uputchar(int ch) {
    char c = (unsigned char) ch;
    if(write(STDOUT_FILENO, &c, 1) < 0) {
        return EOF;
    } else {
        return ch;
    }
}


int ugetchar(void) {
    char ch;
    if(read(STDIN_FILENO, &ch, 1) == 1) {
        return ch;
    } else {
        return EOF;
    }
}


int term_width(void) {
    return t_width;
}


int term_height(void) {
    return t_height;
}


char tty_buffer[TTY_BUFF_SIZE];
size_t buff_idx = 0;

void tputchar(int ch) {
    if (buff_idx >= TTY_BUFF_SIZE) {
        refresh();
    }
    tty_buffer[buff_idx] = (char) ch;
    ++buff_idx;
}


void twritestr(const char *str) {
    size_t len = strlen(str);
    if (len >= TTY_BUFF_SIZE) {
        refresh();
        write(STDOUT_FILENO, str, len);
        return;
    }

    if (buff_idx + len >= TTY_BUFF_SIZE) {
        refresh();
    }
    memcpy(tty_buffer+buff_idx, str, len);
    buff_idx += len;
}


void tprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char *strp;
    vasprintf(&strp, fmt, args);
    twritestr(strp);
    free(strp);

    va_end(args);
}


void refresh(void) {
    write(STDOUT_FILENO, tty_buffer, buff_idx);
    buff_idx = 0;
}
