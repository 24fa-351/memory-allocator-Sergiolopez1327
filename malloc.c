#include "malloc.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE sizeof(BlockHeader)
#define ALIGNMENT 8

typedef struct BlockHeader {
    size_t size;
    int free;
    struct BlockHeader* next;
} BlockHeader;

static BlockHeader* free_list = NULL;
static pthread_mutex_t malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

static size_t align_size(size_t size) {
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

// Function to allocate a chunk of memory using sbrk
void* get_me_blocks(size_t size) {
    void* block = sbrk(size);
    if (block == (void*) -1) {
        perror("sbrk failed");
        return NULL;
    }
    return block;
}

// Updated extend_heap function to use get_me_blocks
static BlockHeader* extend_heap(size_t size) {
    void* block = get_me_blocks(size + BLOCK_SIZE);
    if (!block) {
        return NULL;
    }
    BlockHeader* header = (BlockHeader*) block;
    header->size = size;
    header->free = 0;
    header->next = NULL;
    return header;
}

static BlockHeader* find_free_block(size_t size) {
    BlockHeader* current = free_list;
    while (current) {
        if (current->free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void split_block(BlockHeader* block, size_t size) {
    if (block->size <= size + BLOCK_SIZE)
        return;

    BlockHeader* new_block = (BlockHeader*) ((char*) block + BLOCK_SIZE + size);
    new_block->size = block->size - size - BLOCK_SIZE;
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}

void* my_malloc(size_t size) {
    pthread_mutex_lock(&malloc_mutex);

    size = align_size(size);
    BlockHeader* block = find_free_block(size);

    if (!block) {
        block = extend_heap(size);
        if (!block) {
            pthread_mutex_unlock(&malloc_mutex);
            return NULL;
        }
    } else {
        block->free = 0;
        split_block(block, size);
    }

    pthread_mutex_unlock(&malloc_mutex);
    return (void*) (block + 1);
}

void my_free(void* ptr) {
    if (!ptr)
        return;

    pthread_mutex_lock(&malloc_mutex);

    BlockHeader* block = (BlockHeader*) ptr - 1;
    block->free = 1;

    pthread_mutex_unlock(&malloc_mutex);
}

void* my_realloc(void* ptr, size_t size) {
    if (!ptr)
        return my_malloc(size);

    BlockHeader* block = (BlockHeader*) ptr - 1;
    if (block->size >= size)
        return ptr;

    void* new_ptr = my_malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size);
        my_free(ptr);
    }
    return new_ptr;
}
