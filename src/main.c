#include "tty.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "buffer.h"
#include "editor.h"


int main(int argc, char *argv[]) {
    struct gbuff buff;
    gbuff_creat(&buff, 64);

    // read the file if given one
    if (argc >= 2) {
        int idx = 0;
        //TODO: read directly into buff
        char rbuffer[1024];

        int fd = open(argv[1], O_RDONLY|O_CREAT, 0666);

        int r;
        while ((r=read(fd, rbuffer, 1024)) > 0) {
            gbuff_insert(&buff, idx, r, rbuffer);
            idx += r;
        }
    }

    pi_edit(&buff);

    if (argc >= 2) {
        int fd = open(argv[1], O_WRONLY);

        struct gapstr str = gbuff_read(&buff, 0, -1);

        write(fd, str.part1, str.len1);
        write(fd, str.part2, str.len2);
    }

    gbuff_free(&buff);
    return 0;
}
