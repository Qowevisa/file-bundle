#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

enum state {
    idle,
    created,
    deleted
};

uint8_t file_state = idle;
char path[32] = {0};

void clean_file() {
    if (file_state == created) {
        remove(path);
    }
}

int main(int argc, char *argv[]) {
    atexit(clean_file);
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <FILE> <NUM>\n"
                        "  change every '\\t' char in <FILE>\n"
                        "   <NUM> spaces. <NUM> has to be more than 0.\n", argv[0]);
        return 1;
    }
    // Validating file
    struct stat buffer;
    if (stat(argv[1], &buffer) != 0) {
        fprintf(stderr, "%s: %s: %s\n", argv[0], argv[1], strerror(errno));
        return 2;
    }
    if (!S_ISREG(buffer.st_mode)) {
        fprintf(stderr, "%s: %s: Not a regular file\n", argv[0], argv[1]);
        return 3;
    }
    // Validating num
    uint8_t num = atoi(argv[2]) % 256;
    if (num <= 0) {
        fprintf(stderr, "%s: %s: Not a valid unsigned int value\n", argv[0], argv[2]);
        return 4;
    }
    // creating tmp file
    strcat(path, ".__");
    strncat(path, argv[1], 10);
    strcat(path, ".tmp");
    FILE *tmp_file = fopen(path, "w");
    file_state = created;
    FILE *file = fopen(argv[1], "r");
    int8_t  c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\t') {
            for (uint8_t i = 0; i < num; i++) {
                fputc(' ', tmp_file);
            }
        } else {
            fputc(c, tmp_file);
        }
    }
    if (rename(path, argv[1]) != 0) {
        fprintf(stderr, "%s: rename error: %s\n", argv[0], strerror(errno));
        return 5;
    }
    file_state = deleted;
    return 0;
}
