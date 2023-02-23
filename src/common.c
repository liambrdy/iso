#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

char *readEntireFile(const char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Failed to open file %s: %s\n", fileName, strerror(errno));
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buffer = malloc(size + 1);
    memset(buffer, 0, size + 1);
    fread(buffer, size, 1, f);
    buffer[size] = '\0';

    fclose(f);

    return buffer;
}