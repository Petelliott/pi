#ifndef TTY_H
#define TTY_H

void raw_on(void);
void raw_off(void);

void alternate_terminal(void);
void original_terminal(void);

void clear(void);

void init(void);
void cleanup(void);

void save_cursor(void);
void restore_cursor(void);

void cursor_to(int x, int y);
void cursor_up(void);
void cursor_down(void);
void cursor_left(void);
void cursor_right(void);
void cursor_x(int x);
void cursor_y(int y);

int uputchar(int ch);
int ugetchar();

#endif
