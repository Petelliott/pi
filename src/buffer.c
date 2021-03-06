#include "buffer.h"
#include <stdlib.h>
#include <string.h>


void gbuff_creat(struct gbuff *buff, size_t gap_size) {
    //TODO: consider initializing the buffer with len 0
    buff->base = malloc(gap_size);
    buff->len = gap_size;
    buff->gap_start = 0;
    buff->gap_end = gap_size;
    buff->gap_size = gap_size;
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

    if (len == -1) {
        len = gbuff_len(buff);
    }

    if (start < buff->gap_start) {
        res.part1 = buff->base + start;
        if (start + len < buff->gap_start) {
            res.len1 = len;
            return res;
        } else {
            res.len1 = buff->gap_start - start;
        }
        start += res.len1;
        len   -= res.len1;
    }

    size_t gap_size = buff->gap_end - buff->gap_start;

    // if we are here, we need a second part
    res.part2 = buff->base + start + gap_size;
    res.len2 = len;
    if (buff->gap_end + res.len2 > buff->len) {
        res.len2 = buff->len - buff->gap_end;
    }

    return res;
}


/* moves the gap to loc */
static void gap_to(struct gbuff *buff, size_t loc) {
    if (loc < buff->gap_start) {
        size_t len = buff->gap_start - loc;
        char *new_gap = buff->base + loc;
        char *new_data = buff->base + buff->gap_end - len;
        memmove(new_data, new_gap, len);

        buff->gap_start = loc;
        buff->gap_end   -= len;
    } else if (loc > buff->gap_start) {
        size_t len = loc - buff->gap_start;
        char *new_gap = buff->base + buff->gap_end;
        char *new_data = buff->base + buff->gap_start;
        memmove(new_data, new_gap, len);

        buff->gap_start = loc;
        buff->gap_end   += len;
    }
    // if neither if runs, the gap was already
    // where it needed to be.
}


void gbuff_insert(struct gbuff *buff, size_t start, size_t len, const char *src) {
    size_t gap_len = buff->gap_end - buff->gap_start;
    if (len > gap_len) {
        // reallocate the buffer to fit our string, and have
        // gap_size extra space
        size_t extra_len = len + (buff->gap_size - gap_len);
        gap_to(buff, buff->len - gap_len);
        buff->base = realloc(buff->base, buff->len + extra_len);
        buff->gap_end += extra_len;
        buff->len += extra_len;
    }

    gap_to(buff, start);
    // start == buff->gap_start
    memcpy(buff->base + start, src, len);
    buff->gap_start += len;
}



void gbuff_erase(struct gbuff *buff, size_t start, size_t len) {
    // if we are erasing next to the gap, expand the gap
    if (start <= buff->gap_start && start+len >= buff->gap_start) {
        buff->gap_end = (start + len) + (buff->gap_end - buff->gap_start);
        buff->gap_start = start;
    } else {
        gap_to(buff, start);
        buff->gap_end += len;
    }
}


char gbuff_get(struct gbuff *buff, size_t idx) {
    if (idx < buff->gap_start) {
        return buff->base[idx];
    } else if (idx < gbuff_len(buff)) {
        return buff->base[idx + (buff->gap_end - buff->gap_start)];
    }
    return '\0';
}

void gbuff_add(struct gbuff *buff, size_t idx, char ch) {
    //TODO: specialized function
    gbuff_insert(buff, idx, 1, &ch);
}


void gbuff_del(struct gbuff *buff, size_t idx) {
    //TODO: specialized function maybe
    gbuff_erase(buff, idx, 1);
}
