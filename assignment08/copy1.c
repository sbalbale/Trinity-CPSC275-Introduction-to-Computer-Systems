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
    if (argc < 3 || argc > 4) {     // Check for correct number of arguments
        fprintf(stderr, "Usage: %s srcfile destfile [buffer_size]\n", argv[0]);
        return 1;
    }

    const char *src_filename = argv[1]; // Source filename
    const char *dest_filename = argv[2]; // Destination filename
    int buffer_size = DEFAULT_BUFFER_SIZE; // Default buffer size

    if (argc == 4) {                // Check if buffer size is provided
        buffer_size = atoi(argv[3]); // Parse buffer size
        if (buffer_size <= 0) {     // Validate buffer size
            fprintf(stderr, "Invalid buffer size: %d\n", buffer_size);
            return 1;
        }
    }

    char *buffer = malloc(buffer_size); // Allocate buffer
    if (!buffer) {                  // Check for allocation failure
        perror("malloc");
        return 1;
    }

    int src_fd = open(src_filename, O_RDONLY | O_BINARY); // Open source file
    if (src_fd < 0) {               // Check for open error
        perror("Error opening source file");
        free(buffer);
        return 1;
    }

    // 0666 gives read/write permission to owner, group, and others (modified by umask)
    int dest_fd = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666); // Open destination file
    if (dest_fd < 0) {              // Check for open error
        perror("Error opening destination file");
        close(src_fd);
        free(buffer);
        return 1;
    }

    struct timespec start, end;
    ssize_t bytes_read, bytes_written;

    // Record start time
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) { // Get start time
        perror("clock_gettime");
        close(src_fd);
        close(dest_fd);
        free(buffer);
        return 1;
    }

    // Copy loop
    while ((bytes_read = read(src_fd, buffer, buffer_size)) > 0) { // Read from source
        char *ptr = buffer;         // Pointer to buffer
        ssize_t remaining = bytes_read; // Bytes remaining to write
        while (remaining > 0) {     // Write loop
            bytes_written = write(dest_fd, ptr, remaining); // Write to destination
            if (bytes_written < 0) { // Check for write error
                perror("Error writing to destination file");
                close(src_fd);
                close(dest_fd);
                free(buffer);
                return 1;
            }
            ptr += bytes_written;   // Advance pointer
            remaining -= bytes_written; // Decrease remaining bytes
        }
    }

    if (bytes_read < 0) {           // Check for read error
        perror("Error reading from source file");
    }

    // Record end time
    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) { // Get end time
        perror("clock_gettime");
        close(src_fd);
        close(dest_fd);
        free(buffer);
        return 1;
    }

    close(src_fd);                  // Close source file
    close(dest_fd);                 // Close destination file
    free(buffer);                   // Free buffer

    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0; // Calculate elapsed time
    printf("Total execution time: %.3f milliseconds\n", elapsed_ms); // Print execution time

    return 0;
}
