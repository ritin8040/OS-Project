#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_BLOCKS 100
#define BLOCK_SIZE 1024

typedef struct {
    int pid;
    int start;
    int size;
} block;

int allocate_block(block* memory, int num_blocks, int size);
void deallocate_block(block* memory, int num_blocks, int pid);

int main() {
    // Initialize memory blocks
    block memory[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; i++) {
        memory[i].pid = -1;
        memory[i].start = i * BLOCK_SIZE;
        memory[i].size = BLOCK_SIZE;
    }

    // Set up random number generator
    srand(time(NULL));

    // Simulate memory allocation and deallocation for 10 time units
    int num_wasted_blocks = 0;
    int total_fragmentation = 0;
    for (int t = 1; t <= 10; t++) {
        int num_allocated_blocks = 0;
        int total_allocated_size = 0;
        int num_deallocated_blocks = 0;

        // Allocate memory for new processes
        for (int i = 0; i < 10; i++) {
            int size = rand() % (BLOCK_SIZE / 2) + 1; // Random size up to half a block
            int pid = allocate_block(memory, NUM_BLOCKS, size);
            if (pid != -1) {
                num_allocated_blocks++;
                total_allocated_size += size;
            }
        }

        // Deallocate memory for completed processes
        for (int i = 0; i < NUM_BLOCKS; i++) {
            if (memory[i].pid != -1 && rand() % 10 == 0) { // 10% chance of deallocating
                deallocate_block(memory, NUM_BLOCKS, memory[i].pid);
                num_deallocated_blocks++;
            }
        }

        // Calculate fragmentation and wasted memory
        int num_free_blocks = 0;
        int total_free_size = 0;
        int max_free_block_size = 0;
        int curr_free_block_size = 0;
        for (int i = 0; i < NUM_BLOCKS; i++) {
            if (memory[i].pid == -1) {
                num_free_blocks++;
                total_free_size += memory[i].size;
                curr_free_block_size += memory[i].size;
                if (memory[i].size > max_free_block_size) {
                    max_free_block_size = memory[i].size;
                }
            } else {
                if (curr_free_block_size > 0 && curr_free_block_size < max_free_block_size) {
                    total_fragmentation += max_free_block_size - curr_free_block_size;
                }
                curr_free_block_size = 0;
            }
        }
        if (curr_free_block_size > 0 && curr_free_block_size < max_free_block_size) {
            total_fragmentation += max_free_block_size - curr_free_block_size;
        }
        num_wasted_blocks += num_free_blocks - 1;

        // Print results for this time unit
        printf("Time unit %d:\n", t);
        printf("  Allocated %d blocks (%d bytes)\n", num_allocated_blocks, total_allocated_size);
        printf("  Deallocated %d blocks\n", num_deallocated_blocks);
        printf("  Fragmentation: %d bytes\n", total_fragmentation);
        printf("  Wasted blocks: %d\n", num_wasted_blocks);
    }

    return 0;
}

int allocate_block(block* memory, int num_blocks, int size) {
    for (int i = 0; i < num_blocks; i++) {
        if (memory[i].pid == -1 && memory[i].size >= size) {
            // Allocate block
            memory[i].pid = rand() % 1000 + 1; // Random process ID
            if (memory[i].size > size) {
                // Split block
                block free_block = {
                    -1,
                    memory[i].start + size,
                    memory[i].size - size
                };
                memory[i].size = size;
                // Shift remaining blocks to the right
                for (int j = num_blocks - 1; j > i; j--) {
                    memory[j] = memory[j-1];
                }
                memory[i+1] = free_block;
            }
            return memory[i].pid;
        }
    }
    return -1; // No block found
}

void deallocate_block(block* memory, int num_blocks, int pid) {
    for (int i = 0; i < num_blocks; i++) {
        if (memory[i].pid == pid) {
            // Deallocate block
            memory[i].pid = -1;
            // Merge adjacent free blocks
            if (i > 0 && memory[i-1].pid == -1) {
                memory[i-1].size += memory[i].size;
                // Shift remaining blocks to the left
                for (int j = i; j < num_blocks - 1; j++) {
                    memory[j] = memory[j+1];
                }
                memory[num_blocks-1].pid = -1;
            }
            if (i < num_blocks - 1 && memory[i+1].pid == -1) {
                memory[i].size += memory[i+1].size;
                // Shift remaining blocks to the left
                for (int j = i+1; j < num_blocks - 1; j++) {
                    memory[j] = memory[j+1];
                }
                memory[num_blocks-1].pid = -1;
            }
            break;
        }
    }
}
