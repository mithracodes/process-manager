#include "read.h"
#include <limits.h>

//represents a free memory block
typedef struct {
    int seq;
    int size;
} MemoryBlock;

// function declarations
void allocate_memory(int *num_blocks, MemoryBlock *memory_blocks, Process *process, int *processes_ready, int current_sim_time);
void free_memory_block(int *num_blocks,MemoryBlock *memory_blocks, int memory_address, int memory_size);