#include "tty.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "buffer.h"
#include <string.h>


void print_gapstr(struct gapstr g) {
    if (g.len1 > 0) {
        write(STDOUT_FILENO, g.part1, g.len1);
    }

    if (g.len2 > 0) {
        write(STDOUT_FILENO, g.part2, g.len2);
    }
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

    struct gbuff buff = gbuff_creat(8);

    char test[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    memcpy(buff.base, test, 8);

    buff.gap_start = 3;
    buff.gap_end   = 6;

    print_gapstr(gbuff_read(&buff, 0, 2));
    printf("\n");
    print_gapstr(gbuff_read(&buff, 6, 2));
    printf("\n");
    print_gapstr(gbuff_read(&buff, 0, 5));
    printf("\n");
    print_gapstr(gbuff_read(&buff, 0, 200));
    printf("\n");

    return 0;
}
