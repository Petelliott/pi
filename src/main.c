#include "tty.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "buffer.h"
#include <string.h>
#include <ctype.h>


void print_gapstr(struct gapstr g) {
    if (g.len1 > 0) {
        write(STDOUT_FILENO, g.part1, g.len1);
    }

    if (g.len2 > 0) {
        write(STDOUT_FILENO, g.part2, g.len2);
    }
    printf("\n");
}

void write_buff(const struct gbuff *buff) {
    printf("\"");
    for (int i = 0; i < buff->len; ++i) {
        char ch = buff->base[i];
        if (isprint(ch)) {
            putchar(ch);
        } else {
            putchar('?');
        }
    }
    printf("\"\n");
}

int main(int argc, char* argv[]) {
    /*
    init();

    int c;
    while ((c=getchar()) != 3) {
        printf("%i\n", c);
    }

    cleanup();
    */

    struct gbuff buff;
    gbuff_creat(&buff, 8);

    while (1) {
        size_t idx;
        char in_buffer[1024];

        scanf("%li", &idx);
        fgets(in_buffer, 1024, stdin);
        strtok(in_buffer, "\n");

        gbuff_insert(&buff, idx, strlen(in_buffer), in_buffer);

        print_gapstr(gbuff_read(&buff, 0, -1));
        write_buff(&buff);

    }

    gbuff_free(&buff);
    return 0;
}
