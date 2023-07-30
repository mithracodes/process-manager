#include <string.h>
#include <stdio.h>

// represents a single process
typedef struct Process {
    int arrival_time;
    char* process_name;
    int simulation_time;
    int memory_req;
    int remaining_time;
    int completion_time;
    int has_memory;
    int memory_size;
    int memory_address;
    int finish_time;
} Process;

// function declarations
void parse_arguments(char** argv, int* scheduler, int* mem_strategy, int* quantum, FILE** fp);
void print_metrics(Process *processes, int total_processes, int quantum, int current_sim_time);