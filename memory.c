#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

// This function allocates memory for a process using the best-fit algorithm
void allocate_memory(int *num_blocks, MemoryBlock *memory_blocks, Process *process, int *processes_ready, int current_sim_time) {

    // check if process needs memory
    if (process->has_memory==0) {

        // find the best-fit block of memory
        int best_fit_index = -1;
        int best_fit_size = INT_MAX;

        for (int j = 0; j < (*num_blocks); j++) {

            // check if the memory block is big enough for the process and smaller than the current best fit
            if (memory_blocks[j].size >= process->memory_req && memory_blocks[j].size < best_fit_size) {
                best_fit_index = j;
                best_fit_size = memory_blocks[j].size;
            }
        }

        // if a best-fit block was found, allocate memory for the process
        if (best_fit_index >= 0) {
            process->memory_address = memory_blocks[best_fit_index].seq;
            process->memory_size = process->memory_req;

            // adjust the memory block to reflect the allocation
            memory_blocks[best_fit_index].seq += process->memory_req;
            memory_blocks[best_fit_index].size -= process->memory_req;

            // if the memory block is now empty, remove it from the list of memory blocks
            if (memory_blocks[best_fit_index].size == 0) {
                for (int j = best_fit_index; j < (*num_blocks) - 1; j++) {
                    memory_blocks[j] = memory_blocks[j + 1];
                }
            }

            // if the allocated block is the first block in the list, set the number of blocks to 0
            if (best_fit_index==0) {
                *num_blocks = 0;
            }

            // increment the number of blocks and mark the process as having memory
            (*num_blocks)++;
            process->has_memory = 1;

            // print out a message indicating that the process was assigned memory
            printf("%d,READY,process_name=%s,assigned_at=%d\n", current_sim_time, process->process_name,process->memory_address);
        }
    }
}

// This function frees a block of memory
void free_memory_block(int *num_blocks,MemoryBlock *memory_blocks, int memory_address, int memory_size) {
    // create a new MemoryBlock to represent the memory being freed
    MemoryBlock freed_block = {memory_address, memory_size};

    int j;

    // find the correct index in memory_blocks to insert the freed_block
    for (j = 0; j < (*num_blocks); j++) {
        if (memory_blocks[j].seq > memory_address) {
            break;
        }
    }

    // shift all memory_blocks after the freed_block up one index
    for (int k = (*num_blocks); k > j; k--) {
        memory_blocks[k] = memory_blocks[k - 1];
    }    

    // insert the freed_block into the memory_blocks array
    memory_blocks[j] = freed_block;

    (*num_blocks)++;

    // check for any adjacent memory blocks and combine them
    for (int k = 0; k < (*num_blocks) - 1; k++) {
        if (memory_blocks[k].seq + memory_blocks[k].size == memory_blocks[k + 1].seq) {
            // combine adjacent memory blocks
            memory_blocks[k].size += memory_blocks[k + 1].size;
            // shift all memory_blocks after the merged block back one index
            for (int l = k + 1; l < (*num_blocks) - 1; l++) {
                memory_blocks[l] = memory_blocks[l + 1];
            }
            // reduce the number of blocks by one
            (*num_blocks)--;
            // check the same index again for additional adjacent blocks
            k--;
        }
    }
}