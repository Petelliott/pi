#include "editor.h"
#include "tty.h"
#include <stdio.h>


void pi_edit(struct gbuff *buff) {
    // our position in the buffer
    int crsr = 0;
    int row = 0;
    int draw_row = 0;

    init();

    // redraw bcause the buffer could already contain data
    pi_redraw(buff, crsr, draw_row);

    int c;
    while ((c=ugetchar()) != 3) {
        int redraw_flag = 0;
        if (c == '\e') {
            ugetchar(); // '['
            switch (ugetchar()) {
                case 'A':
                    if (up_line(buff, &crsr)) {
                        row--;
                    }
                    break;
                case 'B':
                    if (down_line(buff, &crsr)) {
                        row++;
                    }
                    break;
                case 'C':
                    if (gbuff_get(buff, crsr) == '\n') {
                        ++row;
                    }
                    ++crsr;
                    break;
                case 'D':
                    --crsr;
                    if (gbuff_get(buff, crsr) == '\n') {
                        --row;
                    }
                    break;
            }
        } else if (c == 127) {
            if (crsr > 0) {
                if (gbuff_get(buff, crsr-1) == '\n') {
                    --row;
                }
                gbuff_del(buff, crsr-1);
                --crsr;
                redraw_flag = 1;
            }
        } else {
            gbuff_add(buff, crsr, c);
            ++crsr;
            if (c == '\n') {
                ++row;
            }
            redraw_flag = 1;
        }

        if (row-draw_row >= term_height()) {
            ++draw_row;
            redraw_flag = 1;
        } else if (row-draw_row < 0) {
            --draw_row;
            redraw_flag = 1;
        }

        crsr = (crsr < 0)? 0:crsr;
        int max = gbuff_len(buff);
        crsr = (crsr > max)? max:crsr;

        // TODO: better dymanic redraw
        if (redraw_flag) {
            pi_redraw(buff, crsr, draw_row);
        } else {
            pi_cursor(buff, crsr, draw_row);
        }
    }

    cleanup();
}


void pi_redraw(struct gbuff *buff, int crsr, int row) {
    clear();
    cursor_to(0, 0);

    int cx = 0;
    int cy = 0;
    int c_set = 0;

    int curr_row = 0;

    for (size_t i = 0; i < gbuff_len(buff); ++i) {
        char ch = gbuff_get(buff, i);

        int in_screen = curr_row >= row && curr_row < (row + term_height()) - 1;
        int last_line = curr_row == (row + term_height()) - 1 && ch != '\n';
        if (in_screen || last_line) {
            tputchar(ch);
        }

        if (ch == '\n') {
            ++curr_row;
            // exit early when we are past the screen
            if (curr_row >= row + term_height()) {
                break;
            }
        }

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
    cursor_to(cx, cy - row);
    refresh();
}


void pi_cursor(struct gbuff *buff, int crsr, int row) {
    int cx = 0;
    int cy = 0;

    for (size_t i = 0; i < gbuff_len(buff); ++i) {
        char ch = gbuff_get(buff, i);

        if (i == crsr) {
            break;
        }

        ++cx;
        if (ch == '\n') {
            cx = 0;
            ++cy;
        }
    }
    cursor_to(cx, cy - row);
    refresh();
}


//TODO: improve
int up_line(struct gbuff *buff, int *crsr) {
    ssize_t l_len = -1;
    ssize_t l_strt;
    for (l_strt = *crsr-1; l_strt >= 0; --l_strt) {
        if (gbuff_get(buff, l_strt) == '\n') {
            l_len = *crsr - l_strt;
            break;
        }
    }

    if (l_len == -1) {
        return 0;
    }

    ssize_t p_line;
    for (p_line = *crsr - l_len - 1; p_line >= 0; --p_line) {
        if (gbuff_get(buff, p_line) == '\n') {
            break;
        }
    }

    ssize_t max = *crsr - l_len;
    ssize_t pos = p_line + l_len;
    *crsr = (pos > max)? max:pos;

    return 1;
}


//TODO: improve
int down_line(struct gbuff *buff, int *crsr) {
    ssize_t l_strt;
    for (l_strt = *crsr-1; l_strt >= 0; --l_strt) {
        if (gbuff_get(buff, l_strt) == '\n') {
            break;
        }
    }
    ssize_t l_len = *crsr - l_strt - 1;

    ssize_t p_start;
    int end = 1;
    for (p_start = *crsr; p_start < gbuff_len(buff); ++p_start) {
        if (gbuff_get(buff, p_start) == '\n') {
            ++p_start;
            end = 0;
            break;
        }
    }

    if (end) {
        return 0;
    }

    ssize_t i;
    for (i = 0; i < l_len; ++i) {
        if (gbuff_get(buff, i + p_start) == '\n') {
            break;
        }
    }
    *crsr = p_start + i;

    return 1;
}
