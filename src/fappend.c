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
        fprintf(stderr, "Usage: %s <FILE> {<STRING>, ...}\n"
                        "  appends every argv after file into the end of file.\n", argv[0]);
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
    FILE *file = fopen(argv[1], "a");
    // appending
    for (int i = 2; i < argc; i++) {
        fprintf(file, "%s\n", argv[i]);
    }
    return 0;
}
