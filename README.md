/*
 * Custom Heap-Based Memory Manager
 *
 * Project Description:
 * This project implements a heap-based memory manager, providing custom versions of
 * malloc, free, and realloc functions. The memory manager acquires a dedicated block
 * of memory and manages allocations within this space. The goal is to handle memory
 * efficiently using a min-heap for free blocks in the final version, starting with
 * simpler data structures such as an array during incremental development.
 *
 * Features:
 * - Custom memory allocation (malloc).
 * - Memory deallocation (free).
 * - Reallocation of memory (realloc).
 * - Thread safety (extra credit).
 * - A test program to validate the functionality.
 *
 * Development Strategy:
 * 1. Incremental Development:
 *    - Begin with a simple test program using standard malloc and free.
 *    - Replace malloc/free with custom versions using an array to track blocks.
 *    - Replace the array with a min-heap for better performance.
 *    - Attach block size and metadata to each allocated memory block.
 *    - Extend tests to cover a variety of scenarios (small/large blocks, reallocations).
 *
 * 2. Testing:
 *    - Comprehensive tests for allocations, frees, and reallocations.
 *    - Include edge cases and stress tests for memory fragmentation.
 *    - Use patterns to verify data integrity in allocated memory.
 *
 * Example Test Code:
 *
 * void* get_me_blocks(ssize_t how_much) {
 *     void* ptr = sbrk(0);
 *     sbrk(how_much);
 *     return ptr;
 * }
 *
 * This function acquires memory for the allocator, requesting a block from the system
 * and returning it for management by the custom allocator.
 *
 * Module Files:
 * 1. malloc.h - Header file declaring the custom memory management functions.
 * 2. malloc.c - Implementation of malloc, free, realloc, and supporting logic.
 *
 * Makefile:
 * The Makefile supports two targets:
 * - Default: Links the test program with the system allocator.
 * - Custom: Links the test program with the custom allocator.
 *
 * Running the Tests:
 * 1. Default test string:
 *    make run
 *
 * 2. Custom test string:
 *    make run ARGS="Custom test string for allocation testing!"
 *
 * Output:
 * The results of each test are stored in results.txt, including:
 * - Allocation sizes.
 * - Memory content for validation.
 * - Pass/fail status of each test.
 *
 * Cleanup:
 * To remove compiled files and outputs:
 * make clean
 *
 * Extra Credit:
 * - Implement thread safety using mutexes to synchronize operations.
 * - Add test cases demonstrating correct behavior under concurrent access.
 *
 * Acknowledgements:
 * This project is an opportunity to gain a deeper understanding of memory management
 * in systems programming. It involves implementing custom dynamic memory allocation
 * and optimizing with efficient data structures to simulate real-world challenges.
 */
