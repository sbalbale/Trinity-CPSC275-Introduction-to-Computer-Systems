/**
* File: mycache.c
* Purpose: simulate a cache memory system with LRU replacement policy
* Author: Sean Balbale
* Date: 11/18/2025
**/


#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// Cache line structure
typedef struct {
    int valid;              // Valid bit
    unsigned long tag;      // Tag bits
    int lru_counter;        // LRU counter for replacement policy
} CacheLine;

// Cache set structure
typedef struct {
    CacheLine *lines;       // Array of cache lines in the set
} CacheSet;

// Cache structure
typedef struct {
    int S;                  // Number of sets (2^s)
    int E;                  // Number of lines per set (associativity)
    int B;                  // Block size (2^b)
    int s;                  // Number of set index bits
    int b;                  // Number of block offset bits
    CacheSet *sets;         // Array of cache sets
    int hits;               // Count of cache hits (data found in cache)
    int misses;             // Count of cache misses (data not in cache)
    int evictions;          // Count of evictions (valid line replaced)
    int lru_clock;          // Global LRU counter: monotonically increasing timestamp for all accesses
} Cache;

// Function prototypes
Cache* initCache(int s, int E, int b);
void freeCache(Cache *cache);
void accessCache(Cache *cache, unsigned long address, char operation, int verbose);
void printUsage(char *argv[]);

/**
 * Function: initCache
 * Purpose: To dynamically allocate and initialize the cache structure with specified parameters.
 * Parameters: int s - Number of set index bits (S = 2^s is the number of sets).
 *             int E - Associativity (number of lines per set).
 *             int b - Number of block offset bits (B = 2^b is the block size).
 * Returns: Cache* - Pointer to the initialized cache structure.
 */
Cache* initCache(int s, int E, int b) {
    // Allocate memory for the main cache structure
    Cache *cache = (Cache *)malloc(sizeof(Cache));
    if (cache == NULL) {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    
    // Initialize cache parameters
    cache->s = s;           // Number of set index bits
    cache->E = E;           // Number of lines per set
    cache->b = b;           // Number of block offset bits
    cache->S = 1 << s;      // 2^s sets
    cache->B = 1 << b;      // 2^b block size
    
    // Initialize statistics
    cache->hits = 0;        // No hits yet
    cache->misses = 0;      // No misses yet
    cache->evictions = 0;   // No evictions yet
    cache->lru_clock = 0;   // Start LRU counter at 0
    
    // Allocate memory for sets
    cache->sets = (CacheSet *)malloc(cache->S * sizeof(CacheSet));
    if (cache->sets == NULL) {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    
    // Allocate memory for lines in each set
    for (int i = 0; i < cache->S; i++) {
        // Allocate E lines for this set
        cache->sets[i].lines = (CacheLine *)malloc(E * sizeof(CacheLine));
        if (cache->sets[i].lines == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            exit(1);
        }
        
        // Initialize each line
        for (int j = 0; j < E; j++) {
            cache->sets[i].lines[j].valid = 0;        // Mark as invalid (empty)
            cache->sets[i].lines[j].tag = 0;          // Clear tag
            cache->sets[i].lines[j].lru_counter = 0;  // Reset LRU timestamp
        }
    }
    
    return cache;
}

/**
 * Function: freeCache
 * Purpose: To free all dynamically allocated memory used by the cache structure.
 * Parameters: Cache *cache - Pointer to the cache structure to be freed.
 * Returns: void
 */
void freeCache(Cache *cache) {
    if (cache == NULL) return;
    
    // Free each set's lines
    for (int i = 0; i < cache->S; i++) {
        free(cache->sets[i].lines);
    }
    
    // Free the sets array
    free(cache->sets);
    
    // Free the cache structure
    free(cache);
}

/**
 * Function: accessCache
 * Purpose: To simulate a cache memory access with the given address and operation type,
 *          tracking hits, misses, and evictions using LRU replacement policy.
 * Parameters: Cache *cache - Pointer to the cache structure.
 *             unsigned long address - Memory address to access.
 *             char operation - Operation type ('L' for load, 'S' for store, 'M' for modify).
 *             int verbose - Flag for verbose output mode (1 to enable, 0 to disable).
 * Returns: void
 */
void accessCache(Cache *cache, unsigned long address, char operation, int verbose) {
    // Extract set index and tag from memory address
    // Address format: [tag bits][set index bits][block offset bits]
    unsigned long set_mask = (1UL << cache->s) - 1;           // Create mask with s bits set to 1
    unsigned long set_index = (address >> cache->b) & set_mask; // Shift out block bits, mask for set index
    unsigned long tag = address >> (cache->s + cache->b);     // Shift out both block and set bits to get tag
    
    CacheSet *set = &cache->sets[set_index];
    int hit_line = -1;      // Index of matching line (hit), -1 if no match
    int empty_line = -1;    // Index of first empty line, -1 if all valid
    int lru_line = 0;       // Index of least recently used line (for eviction)
    int min_lru = INT_MAX;  // Minimum LRU counter value seen (oldest access)
    
    // Search for the tag in the set
    for (int i = 0; i < cache->E; i++) {
        if (set->lines[i].valid) {
            // Update LRU tracking
            if (set->lines[i].lru_counter < min_lru) {
                min_lru = set->lines[i].lru_counter;
                lru_line = i;
            }
            
            // Check for cache hit
            if (set->lines[i].tag == tag) {
                hit_line = i;
            }
        } else if (empty_line == -1) {
            // Track first empty line
            empty_line = i;
        }
    }
    
    // Handle cache hit
    if (hit_line != -1) {
        cache->hits++;
        if (verbose) printf("hit ");
        
        // Update LRU counter for the hit line
        set->lines[hit_line].lru_counter = cache->lru_clock++;
        
        // For modify operation, it's a load then store (hit twice)
        if (operation == 'M') {
            cache->hits++;
            if (verbose) printf("hit ");
        }
    }
    // Handle cache miss
    else {
        cache->misses++;
        if (verbose) printf("miss ");
        
        // Check if we need to evict
        if (empty_line == -1) {
            // All lines are valid, need to evict LRU line
            cache->evictions++;
            if (verbose) printf("eviction ");
            
            // Replace LRU line
            set->lines[lru_line].tag = tag;
            set->lines[lru_line].valid = 1;
            set->lines[lru_line].lru_counter = cache->lru_clock++;  // Mark as most recent
        } else {
            // Use empty line
            set->lines[empty_line].tag = tag;
            set->lines[empty_line].valid = 1;
            set->lines[empty_line].lru_counter = cache->lru_clock++;  // Mark as most recent
        }
        
        // For modify operation, second access is a hit
        if (operation == 'M') {
            cache->hits++;
            if (verbose) printf("hit ");
        }
    }
}

/**
 * Function: printUsage
 * Purpose: To display the usage information and command-line options for the cache simulator.
 * Parameters: char *argv[] - Command-line argument array to extract program name.
 * Returns: void
 */
void printUsage(char *argv[]) {
    printf("Usage: %s [-v] -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
    printf("Options:\n");
    printf("  -v         Optional verbose flag that displays trace info\n");
    printf("  -s <s>     Number of set index bits (S = 2^s is the number of sets)\n");
    printf("  -E <E>     Associativity (number of lines per set)\n");
    printf("  -b <b>     Number of block bits (B = 2^b is the block size)\n");
    printf("  -t <file>  Name of the trace file\n");
}

/**
 * Function: main
 * Purpose: To parse command-line arguments, initialize the cache simulator, process trace file,
 *          and display cache performance statistics (hits, misses, evictions).
 * Parameters: int argc - Number of command-line arguments.
 *             char *argv[] - Array of command-line argument strings.
 * Returns: int - Exit status (0 for success, 1 for error).
 */
int main(int argc, char *argv[]) {
    int s = 0, E = 0, b = 0;
    char *tracefile = NULL;
    int verbose = 0;        // Flag for verbose output mode
    int opt;
    
    // Parse command-line arguments
    // Options: -v (verbose), -s <s>, -E <E>, -b <b>, -t <tracefile>
    while ((opt = getopt(argc, argv, "vs:E:b:t:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                tracefile = optarg;
                break;
            default:
                printUsage(argv);
                exit(1);
        }
    }
    
    // Validate required arguments
    if (s <= 0 || E <= 0 || b <= 0 || tracefile == NULL) {
        printUsage(argv);
        exit(1);
    }
    
    // Initialize cache
    Cache *cache = initCache(s, E, b);
    
    // Open trace file
    FILE *fp = fopen(tracefile, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", tracefile);
        freeCache(cache);
        exit(1);
    }
    
    // Process trace file
    char operation;
    unsigned long address;
    int size;
    
    while (fscanf(fp, " %c %lx,%d", &operation, &address, &size) == 3) {
        // Skip instruction loads (I)
        if (operation == 'I') {
            continue;
        }
        
        // Print verbose output if enabled
        if (verbose) {
            printf("%c %lx,%d ", operation, address, size);
        }
        
        // Process the memory access
        accessCache(cache, address, operation, verbose);
        
        // Print newline for verbose output
        if (verbose) {
            printf("\n");
        }
    }
    
    // Close trace file
    fclose(fp);
    
    // Print results
    printf("hits:%d misses:%d evictions:%d\n", cache->hits, cache->misses, cache->evictions);
    
    // Clean up
    freeCache(cache);
    
    return 0;
}
