#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define HEX "0123456789ABCDEF"

#define print_c(c) {                                    \
    if (state & bytes) {                                \
        printf("%c%c", HEX[past>>4], HEX[past&15]);     \
    } else {                                            \
        printf("%u", past);                             \
    }                                                   \
}

enum options {
    hex = 1
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Usage: %s [OPTIONS] <FILE>\n"
                        "  prints set of bytes from file\n"
                        "  set starts from the lowest that is actually used\n"
                        "  for example, if file F contains only 'A' bytes the output\n"
                        "    will be just 65 or 41 if -h is set\n"
                        "  OPTIONS:\n"
                        "    -h   : hex variant of bytes in set (from 00 to FF)\n", argv[0]);
        return 1;
    }
    uint32_t state = 0;
    // getting opts
    for (int32_t i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int8_t c;
            for (uint32_t l = 1; l < strlen(argv[i]); l++) {
                c = argv[i][l];
                switch (c) {
                    case 'h':
                        state |= hex;
                        break;
                }
            }
        }
    }
    // Validating file
    int32_t fi = 1;
    for (int32_t i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            fi++;
        } else {
            break;
        }
    }
    struct stat buffer;
    if (stat(argv[fi], &buffer) != 0) {
        fprintf(stderr, "%s: %s: %s\n", argv[0], argv[fi], strerror(errno));
        return 2;
    }
    if (!S_ISREG(buffer.st_mode)) {
        fprintf(stderr, "%s: %s: Not a regular file\n", argv[0], argv[fi]);
        return 3;
    }
    FILE *file = fopen(argv[fi], "r");
    //
    uint8_t charset[256] = {0};
    uint8_t c;
    for (uint32_t i = 0; i < buffer.st_size; i++) {
        c = fgetc(file);
        if (!(charset[c])) {
            charset[c]++;
        }
    }
    int32_t past = -1;
    int32_t curr = -1;
    for (uint32_t i = 0; i < 256; i++) {
        if (charset[i]) {
            curr = i;
            if (past != -1) {
                print_c(past);
                printf(" ");
            }
            past = curr;
        }
    }
    print_c(curr);
    printf("\n");
    return 0;
}
