#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

struct gbuff {
    char *base;
    size_t len;
    size_t gap_start;
    size_t gap_end;
};

struct gapstr {
    const char *part1;
    const char *part2;
    size_t len1;
    size_t len2;
};

void gbuff_creat(struct gbuff *buff, size_t init_len);
void gbuff_free(struct gbuff *buff);

struct gapstr gbuff_read(struct gbuff *buff, size_t start, size_t len);
int gbuff_write(struct gbuff *buff, size_t start, size_t len, const char *src);
int gbuff_erase(struct gbuff *buff, size_t start, size_t len);

char buff_get(struct gbuff *buff, size_t idx);
int buff_add(struct gbuff *buff, size_t idx, char ch);
int buff_del(struct gbuff *buff, size_t idx);

#endif
