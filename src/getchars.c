#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <FILE> <NUMBER>\n"
                        "  prints first <NUMBER> bytes in <FILE>\n", argv[0]);
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
    uint32_t size = buffer.st_size;
    FILE *file = fopen(argv[1], "r");
    // getting the chars
    int32_t tmp_n = atoi(argv[2]);
    if (tmp_n <= 0) {
        fprintf(stderr, "%s: %s: Not a valid int number\n", argv[0], argv[2]);
        return 4;
    }
    uint32_t n = (uint32_t)tmp_n;
    uint32_t i = 0;
    char c = fgetc(file);
    while (i < n && i < size) {
        fputc(c, stdout);
        c = fgetc(file);
        i++;
    }
    fputc('\n', stdout);
    return 0;
}
