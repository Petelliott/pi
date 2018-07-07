#include "editor.h"
#include "tty.h"
#include <stdio.h>


void pi_edit(struct gbuff *buff) {
    // our position in the buffer
    int crsr = 0;

    init();

    // redraw bcause the buffer could already contain data
    pi_redraw(buff, crsr);

    int c;
    while ((c=ugetchar()) != 3) {
        if (c == '\e') {
            ugetchar(); // '['
            switch (ugetchar()) {
                case 'A':
                    up_line(buff, &crsr);
                    break;
                case 'B':
                    down_line(buff, &crsr);
                    break;
                case 'C':
                    ++crsr;
                    break;
                case 'D':
                    --crsr;
                    break;
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
            //if c_set, then cx,cy are the cursors coords.
            ++cx;
            if (ch == '\n') {
                cx = 0;
                ++cy;
            }
        }
    }
    cursor_to(cx, cy);
}


//TODO: improve
void up_line(struct gbuff *buff, int *crsr) {
    ssize_t l_len = -1;
    ssize_t l_strt;
    for (l_strt = *crsr-1; l_strt >= 0; --l_strt) {
        if (gbuff_get(buff, l_strt) == '\n') {
            l_len = *crsr - l_strt;
            break;
        }
    }

    if (l_len == -1) {
        return;
    }

    ssize_t p_line;
    for (p_line = *crsr - l_len - 1; p_line >= 0; --p_line) {
        if (gbuff_get(buff, p_line) == '\n') {
            break;
        }
    }

    ssize_t max = (*crsr - l_len - 1) - p_line;
    ssize_t pos = p_line + l_len;
    *crsr = (pos > max)? max:pos;

}


//TODO: improve
void down_line(struct gbuff *buff, int *crsr) {
    ssize_t l_strt;
    for (l_strt = *crsr-1; l_strt >= 0; --l_strt) {
        if (gbuff_get(buff, l_strt) == '\n') {
            break;
        }
    }
    ssize_t l_len = *crsr - l_strt - 1;

    ssize_t p_start;
    for (p_start = *crsr; p_start < gbuff_len(buff); ++p_start) {
        if (gbuff_get(buff, p_start) == '\n') {
            ++p_start;
            break;
        }
    }

    ssize_t i;
    for (i = 0; i < l_len; ++i) {
        if (gbuff_get(buff, i + p_start) == '\n') {
            break;
        }
    }
    *crsr = p_start + i;
}
