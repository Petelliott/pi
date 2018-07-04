#include "tty.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


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
    const char* alt = "\e[?1047h";
    write(STDOUT_FILENO, alt, strlen(alt));
}


void original_terminal(void) {
    const char* orig = "\e[?1047l";
    write(STDOUT_FILENO, orig, strlen(orig));
}


void clear(void) {
    const char* clear_str = "\e[2J";
    write(STDOUT_FILENO, clear_str, strlen(clear_str));
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
    dprintf(STDOUT_FILENO, "\e[s");
}


void restore_cursor(void) {
    dprintf(STDOUT_FILENO, "\e[u");
}


void cursor_to(int x, int y) {
    dprintf(STDOUT_FILENO, "\e[%i;%iH", y, x);
}
