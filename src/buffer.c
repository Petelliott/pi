#include "buffer.h"
#include <stdlib.h>


struct gbuff gbuff_creat(size_t init_len) {
    struct gbuff buff;
    buff.base = malloc(init_len);
    buff.len = init_len;
    buff.gap_start = 0;
    buff.gap_end = init_len;
    return buff;
}


void gbuff_free(struct gbuff *buff) {
    free(buff->base);
    buff->base = NULL;
    buff->len = 0;
    buff->gap_start = 0;
    buff->gap_end = 0;
}


struct gapstr gbuff_read(struct gbuff *buff, size_t start, size_t len) {
    struct gapstr res = {0};

    if (start < buff->gap_start) {
        res.part1 = buff->base + start;
        if (start + len < buff->gap_start) {
            res.len1 = len;
            return res;
        } else {
            res.len1 = buff->gap_start - start;
        }
    }

    if (start+len >= buff->gap_start) {
        res.part2 = buff->base + buff->gap_end;
        res.len2 = (start < buff->gap_start)? (len - res.len1):len;
    }

    return res;
}
