#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s srcfile destfile [buffer_size]\n", argv[0]);
        return 1;
    }

    const char *src_filename = argv[1];
    const char *dest_filename = argv[2];
    int buffer_size = DEFAULT_BUFFER_SIZE;

    if (argc == 4) {
        buffer_size = atoi(argv[3]);
        if (buffer_size <= 0) {
            fprintf(stderr, "Invalid buffer size: %d\n", buffer_size);
            return 1;
        }
    }

    char *buffer = malloc(buffer_size);
    if (!buffer) {
        perror("malloc");
        return 1;
    }

    FILE *src_file = fopen(src_filename, "rb");
    if (!src_file) {
        perror("Error opening source file");
        free(buffer);
        return 1;
    }

    FILE *dest_file = fopen(dest_filename, "wb");
    if (!dest_file) {
        perror("Error opening destination file");
        fclose(src_file);
        free(buffer);
        return 1;
    }

    struct timespec start, end;
    size_t bytes_read, bytes_written;

    // Record start time
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime");
        fclose(src_file);
        fclose(dest_file);
        free(buffer);
        return 1;
    }

    // Copy loop
    while ((bytes_read = fread(buffer, 1, buffer_size, src_file)) > 0) {
        bytes_written = fwrite(buffer, 1, bytes_read, dest_file);
        if (bytes_written < bytes_read) {
            perror("Error writing to destination file");
            fclose(src_file);
            fclose(dest_file);
            free(buffer);
            return 1;
        }
    }

    if (ferror(src_file)) {
        perror("Error reading from source file");
    }

    // Record end time
    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime");
        fclose(src_file);
        fclose(dest_file);
        free(buffer);
        return 1;
    }

    fclose(src_file);
    fclose(dest_file);
    free(buffer);

    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Total execution time: %.3f milliseconds\n", elapsed_ms);

    return 0;
}
