#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "janpatch.h"

#define MAX_FILEREAD_SIZE 524288

int main(int argc, char **argv) {
    if (argc != 3 && argc != 4) {
        printf("Usage: janpatch-cli [old-file] [patch-file] ([new-file])\n");
        return 1;
    }

    // To simulate non-POSIX put data into some buffers
    FILE *old = fopen(argv[1], "rb");
    FILE *patch = fopen(argv[2], "rb");
    FILE *target = argc == 4 ? fopen(argv[3], "wb") : stdout;
    uint8_t *old_data = malloc(MAX_FILEREAD_SIZE * sizeof(char));
    uint8_t *patch_data = malloc(MAX_FILEREAD_SIZE * sizeof(char));
    uint8_t *target_data = malloc(MAX_FILEREAD_SIZE * sizeof(char));

    if (!old) {
        printf("Could not open '%s'\n", argv[1]);
        return 1;
    }
    if (!patch) {
        printf("Could not open '%s'\n", argv[2]);
        return 1;
    }
    if (!target) {
        printf("Could not open '%s'\n", argv[3]);
        return 1;
    }

    fread(old_data, 1, MAX_FILEREAD_SIZE, old);
    fread(patch_data, 1, MAX_FILEREAD_SIZE, patch);

    // janpatch_ctx contains buffers, and references to the file system functions
    janpatch_ctx ctx = {
        { (unsigned char*)malloc(4096), 4096 }, // source buffer
        { (unsigned char*)malloc(4096), 4096 }, // patch buffer
        { (unsigned char*)malloc(4096), 4096 }, // target buffer
    };


    janpatch(ctx, old_data, patch_data, target_data);
    fwrite(target_data, 1, MAX_FILEREAD_SIZE, target);
    return 0;
}
