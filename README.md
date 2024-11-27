# Large Memory Allocator

## Overview
This project involves implementing a **heap-based memory manager**. The module will include the following functions:

- **`malloc`**: Allocate memory and reset it to zero.
- **`free`**: Release allocated memory.
- **`realloc`**: Resize an allocated block of memory.

The memory manager will:
1. Acquire a block of memory to manage.
2. Use structures within this block to handle allocations and deallocations.
3. Implement a **minheap** for free blocks in the final version (initial versions may use arrays for simplicity).

### Extra Credit
Demonstrating and implementing **thread safety** will be rewarded with extra credit.

---

## Module Implementation

### Structure
The memory allocator will consist of:
- A **`.h` file**: Contains declarations for `malloc`, `free`, `realloc`, and related functions.
- A **`.c` file**: Contains the actual implementation.

### Compatibility
The test program should run seamlessly with either the system's standard memory manager or the custom memory manager. No changes to the test program are required.

---

## Incremental Development

### Suggested Steps
1. **Build the Test Program**:
   - Start with a testing framework for `malloc`, `free`, and `realloc`.
2. **Basic Module Implementation**:
   - Create placeholder functions (`malloc`, `free`, `realloc`) that use the system versions.
3. **Custom Memory Manager**:
   - Replace the system functions with your own:
     - Use an **array** to track allocated and free blocks initially.
     - Transition to a **minheap** for free blocks.
4. **Block Metadata**:
   - Attach size metadata to allocated blocks (store this information in a `void*` immediately before the memory chunk).

---

## Testing

### Goals
Testing is critical to ensure correctness and performance:
- Test allocation patterns:
  - Small, large, and same-sized allocations.
  - Interspersed `malloc` and `free` calls.
- Test memory integrity:
  - Fill allocated memory with patterns to verify proper allocation and zeroing.

### Methodology
1. **Test Cases Document**:
   - Write detailed test cases before coding.
2. **Test Program**:
   - Include all test cases.
   - Support a `-t num` command-line option to run a specific test.
   - Output:
     - Test name.
     - Pass/Fail status.

3. **Use System Malloc for Validation**:
   - Start with standard `malloc` for tests.
   - Compare results with the custom implementation.

---

## Makefile

### Targets
- **System Version**:
  - Links the test program with the system’s memory management functions.
- **Custom Version**:
  - Links the test program with the custom memory allocator.

Output for both should be equivalent (apart from randomized elements).

---

## Acquiring a Block of Memory

### Implementation
To manage memory, you will request a dedicated block of memory from the system using the `sbrk` function. Example:

```c
void* get_me_blocks(ssize_t how_much) {
    void* ptr = sbrk(0);
    sbrk(how_much);
    return ptr;
}
