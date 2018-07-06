#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

/* gbuff is a character gap buffer
   base:      pointer to buffer memory area
   len:       size allocated to buffer
   gap_start: start of the gap (inclusice)
   gap_end:   end ot the gap (exculsive)
   gap_size:  target empty space in gap. */
struct gbuff {
    char *base;
    size_t len;
    size_t gap_start;
    size_t gap_end;
    size_t gap_size; // target gap_size
};

/* a non-continuous memory area representing
   a substring of a buffer that may or may not
   cross the gap.
   either len may be zero, and when this occurs
   the values of each part are not guaranteed */
struct gapstr {
    const char *part1;
    const char *part2;
    size_t len1;
    size_t len2;
};

/* allocates the buffer sets up the necessary variables */
void gbuff_creat(struct gbuff *buff, size_t gap_size);
/* frees the buffer and nulls the pointer */
void gbuff_free(struct gbuff *buff);

/* produces a gap_str of the desired substring.
   if len is -1, the remainder of the buffer is returned */
struct gapstr gbuff_read(struct gbuff *buff, size_t start, size_t len);

/* inserts into the buffer before start */
void gbuff_insert(struct gbuff *buff, size_t start, size_t len, const char *src);

/* erases a substring of the buffer */
void gbuff_erase(struct gbuff *buff, size_t start, size_t len);

/* gets the character at idx in the buffer. a null
   character will be returned if idx is out of range */
char gbuff_get(struct gbuff *buff, size_t idx);

/* inserts ch before idx in the buffer */
void gbuff_add(struct gbuff *buff, size_t idx, char ch);

/* deletes the character at idx in the buffer */
void gbuff_del(struct gbuff *buff, size_t idx);

/* gets the in-use length of the buffer */
inline size_t gbuff_len(const struct gbuff *buff) {
    return buff->len - (buff->gap_end - buff->gap_start);
}

#endif
