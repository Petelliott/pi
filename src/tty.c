#include "tty.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>


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

int t_width;
int t_height;
void (*resize_func)(void) = NULL;


static void resize_handler(int sig) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    t_width = w.ws_col;
    t_height = w.ws_row;

    if (resize_func != NULL) {
        (*resize_func)();
    }
}


void set_resize_func(void (*func)(void)) {
    resize_func = func;
}


void clear_resize_func(void) {
    resize_func = NULL;
}


struct sigaction old_action;

void init(void) {
    raw_on();
    alternate_terminal();
    clear();
    save_cursor();
    cursor_to(0, 0);

    resize_handler(SIGWINCH);

    struct sigaction new_action;
    new_action.sa_handler = resize_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGWINCH, &new_action, &old_action);
}


void cleanup(void) {
    sigaction(SIGWINCH, &old_action, NULL);
    clear_resize_func();
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
