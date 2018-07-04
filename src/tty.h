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

#endif
