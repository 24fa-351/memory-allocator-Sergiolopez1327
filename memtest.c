#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef SYSTEM_MALLOC
#define xfree free
#define xmalloc malloc
#define xrealloc realloc
#else
#include "malloc.h"
#endif

int rand_between(int min, int max) { return rand() % (max - min + 1) + min; }

#define TEST_SIZE 30
#define LARGE_ALLOC_PROBABILITY 10  // 10% probability of large allocation
#define REALLOC_PROBABILITY 20      // 20% probability of realloc

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char* argv[]) {
    srand(time(NULL));

    char* test_string =
        "Now is the time for all good people to come to the aid "
        "of their country.";

    if (argc > 1) {
        test_string = argv[1];
    }

    char* ptrs[TEST_SIZE];
    memset(ptrs, 0, sizeof(ptrs));  // Initialize the pointer array to NULL

    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size;

        // Occasionally allocate large chunks
        if (rand_between(1, 100) <= LARGE_ALLOC_PROBABILITY) {
            size = rand_between(1024,
                                1024 * 1024);  // Allocate between 1KB and 1MB
            fprintf(stderr, "[%d] Allocating large chunk of size: %d\n", ix,
                    size);
        } else {
            size = rand_between(1, strlen(test_string) + 1);
            fprintf(stderr, "[%d] Allocating size: %d\n", ix, size);
        }

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            printf("[%d] malloc failed\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%d] Allocated and filled: '%s'\n", ix, ptrs[ix]);

        // Occasionally realloc the buffer (20% chance)
        if (rand_between(1, 100) <= REALLOC_PROBABILITY) {
            int new_size = rand_between(1, strlen(test_string) + 1);
            if (rand_between(1, 100) <= LARGE_ALLOC_PROBABILITY) {
                new_size =
                    rand_between(1024, 1024 * 1024);  // Reallocate large buffer
            }
            fprintf(stderr, "[%d] Reallocating to new size: %d\n", ix,
                    new_size);

            char* new_ptr = xrealloc(ptrs[ix], new_size);
            if (new_ptr == NULL) {
                fprintf(stderr, "[%d] realloc failed\n", ix);
                exit(1);
            }
            ptrs[ix] = new_ptr;

            len_to_copy = MIN(strlen(test_string), new_size - 1);
            strncpy(ptrs[ix], test_string, len_to_copy);
            ptrs[ix][len_to_copy] = '\0';

            fprintf(stderr, "[%d] Reallocated and updated: '%s'\n", ix,
                    ptrs[ix]);
        }

        // Randomly free one of the earlier allocated blocks
        int index_to_free = rand_between(0, ix);
        if (ptrs[index_to_free]) {
            fprintf(stderr, "[%d] Randomly freeing %p ('%s')\n", index_to_free,
                    ptrs[index_to_free], ptrs[index_to_free]);
            xfree(ptrs[index_to_free]);
            fprintf(stderr, "[%d] Freed %p\n", index_to_free,
                    ptrs[index_to_free]);
            ptrs[index_to_free] = NULL;
        }
    }

    // Free any remaining allocations
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        if (ptrs[ix]) {
            fprintf(stderr, "[%d] Freeing %p ('%s')\n", ix, ptrs[ix], ptrs[ix]);
            xfree(ptrs[ix]);
            fprintf(stderr, "[%d] Freed %p\n", ix, ptrs[ix]);
            ptrs[ix] = NULL;
        } else {
            fprintf(stderr, "[%d] Already freed\n", ix);
        }
    }

    return 0;
}
