#include "editor.h"
#include "tty.h"
#include <stdio.h>


void pi_edit(struct gbuff *buff) {
    int crsr = 0;

    init();

    pi_redraw(buff, crsr);

    int c;
    while ((c=ugetchar()) != 3) {
        if (c == '\e') {
            ugetchar(); // '['
            c = ugetchar();
            if (c == 'C') {
                ++crsr;
            } else if (c == 'D') {
                --crsr;
            }
        } else if (c == 127) {
            if (crsr > 0) {
                gbuff_del(buff, crsr-1);
                --crsr;
            }
        } else {
            gbuff_add(buff, crsr, c);
            ++crsr;
        }

        crsr = (crsr < 0)? 0:crsr;
        int max = gbuff_len(buff);
        crsr = (crsr > max)? max:crsr;

        // TODO: dymanic redraw
        pi_redraw(buff, crsr);
    }

    cleanup();
}


void pi_redraw(struct gbuff *buff, int crsr) {
    clear();
    cursor_to(0, 0);

    int cx = 0;
    int cy = 0;
    int c_set = 0;

    for (size_t i = 0; i < gbuff_len(buff); ++i) {
        char ch = gbuff_get(buff, i);
        uputchar(ch);

        c_set = (i==crsr) || c_set;

        if (!c_set) {
            ++cx;
            if (ch == '\n') {
                cx = 0;
                ++cy;
            }
        }
    }
    cursor_to(cx, cy);
}
