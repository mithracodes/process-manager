
#include "memory.h"

//function declarations

void round_robin_scheduling(Process** ready_processes, int* processes_ready, Process* current_process);
int calc_finished_time(int current_sim_time, int quantum);
void complete_process(Process *processes, Process **ready_processes, int total_processes, Process *current_process, int current_sim_time, int quantum, char* finished_process_name, int *processes_ready);
void update_ready_processes(int *processes_ready, Process **ready_processes);
int sort_ready_processes(const void *p1, const void *p2);
void add_ready_processes(Process *processes, int total_processes, Process **ready_processes, int *processes_ready, int current_sim_time, int quantum, MemoryBlock *memory_blocks, int *num_blocks, int mem_strategy);
int proc_before_completion (Process **ready_processes, Process *processes, int *processes_ready, int current_sim_time, int quantum);
