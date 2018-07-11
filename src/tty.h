#ifndef TTY_H
#define TTY_H

#define TTY_BUFF_SIZE 1024

/* disables echo, cannonical mode, and signals */
void raw_on(void);

/* reverts terminal to version at call of raw_on
   DO NOT CALL BEFORE RAW_ON */
void raw_off(void);

/* switches to alternate terminal 1047 */
void alternate_terminal(void);

/* disables alternate terminal 1047 */
void original_terminal(void);

/* clear the terminal */
void clear(void);

void refresh_term_size(void);

/* sets up the environment for fullscreen tty programs */
void init(void);

/* reverts everything done by init*/
void cleanup(void);

void save_cursor(void);
void restore_cursor(void);

/* moves the cursor the specified position
   zero-indexed, starting from top left
   x is horizontal, y is vertical */
void cursor_to(int x, int y);
void cursor_x(int x);
void cursor_y(int y);

void cursor_up(void);
void cursor_down(void);
void cursor_left(void);
void cursor_right(void);

/* unbuffered versions of putchar() and getchar() */
int uputchar(int ch);
int ugetchar(void);

/* get the terminal width and height */
int term_width(void);
int term_height(void);

/* buffered io */
void tputchar(int ch);
void twritestr(const char *str);
void tprintf(const char *fmt, ...);

void refresh(void);

#endif
