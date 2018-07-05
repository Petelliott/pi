#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

struct gbuff {
    char *base;
    size_t len;
    size_t gap_start;
    size_t gap_end;
    size_t gap_size; // target gap_size
};

struct gapstr {
    const char *part1;
    const char *part2;
    size_t len1;
    size_t len2;
};

void gbuff_creat(struct gbuff *buff, size_t gap_size);
void gbuff_free(struct gbuff *buff);

struct gapstr gbuff_read(struct gbuff *buff, size_t start, size_t len);
void gbuff_insert(struct gbuff *buff, size_t start, size_t len, const char *src);
void gbuff_erase(struct gbuff *buff, size_t start, size_t len);

char buff_get(struct gbuff *buff, size_t idx);
void buff_add(struct gbuff *buff, size_t idx, char ch);
void buff_del(struct gbuff *buff, size_t idx);

#endif
