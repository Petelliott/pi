#ifndef EDITOR_H
#define EDITOR_H

#include "buffer.h"

/* switches to an alternate terminal, and edits
   buff. */
void pi_edit(struct gbuff *buff);

/* redraws the editor state */
void pi_redraw(struct gbuff *buff, int crsr);

void up_line(struct gbuff *buff, int *crsr);
void down_line(struct gbuff *buff, int *crsr);

#endif
