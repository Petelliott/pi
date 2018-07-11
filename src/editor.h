#ifndef EDITOR_H
#define EDITOR_H

#include "buffer.h"

/* switches to an alternate terminal, and edits
   buff. */
void pi_edit(struct gbuff *buff);

/* performs and applies one input operation */
void pi_input(struct gbuff *buff, int *crsr, int *row, int *draw_row, char c);

void pi_arrow_key(struct gbuff *buff, int *crsr, int *row, char op);
void pi_backspace(struct gbuff *buff, int *crsr, int *row);
void pi_insert(struct gbuff *buff, int *crsr, int *row, char c);

/* redraws the editor state */
void pi_redraw(struct gbuff *buff, int crsr, int row);
/* moves the cursor to the desired position */
void pi_cursor(struct gbuff *buff, int crsr, int row);

int up_line(struct gbuff *buff, int *crsr);
int down_line(struct gbuff *buff, int *crsr);

#endif
