#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DEFAULT_BUFFER_SIZE 1024

#ifndef O_BINARY
#define O_BINARY 0
#endif

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

    int src_fd = open(src_filename, O_RDONLY | O_BINARY);
    if (src_fd < 0) {
        perror("Error opening source file");
        free(buffer);
        return 1;
    }

    // 0666 gives read/write permission to owner, group, and others (modified by umask)
    int dest_fd = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666);
    if (dest_fd < 0) {
        perror("Error opening destination file");
        close(src_fd);
        free(buffer);
        return 1;
    }

    struct timespec start, end;
    ssize_t bytes_read, bytes_written;

    // Record start time
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime");
        close(src_fd);
        close(dest_fd);
        free(buffer);
        return 1;
    }

    // Copy loop
    while ((bytes_read = read(src_fd, buffer, buffer_size)) > 0) {
        char *ptr = buffer;
        ssize_t remaining = bytes_read;
        while (remaining > 0) {
            bytes_written = write(dest_fd, ptr, remaining);
            if (bytes_written < 0) {
                perror("Error writing to destination file");
                close(src_fd);
                close(dest_fd);
                free(buffer);
                return 1;
            }
            ptr += bytes_written;
            remaining -= bytes_written;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading from source file");
    }

    // Record end time
    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime");
        close(src_fd);
        close(dest_fd);
        free(buffer);
        return 1;
    }

    close(src_fd);
    close(dest_fd);
    free(buffer);

    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Total execution time: %.3f milliseconds\n", elapsed_ms);

    return 0;
}
