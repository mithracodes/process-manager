#include "scheduling.h"

//Implements the round robin scheduling algrorithm
void round_robin_scheduling(Process** ready_processes, int* processes_ready, Process* current_process) {
    // Check if there are more than one processes ready
    if (*processes_ready > 1) {
        // Move the first process to the end of the ready_processes array
        // and shift all other processes up one index
        for (int i = 0; i < *processes_ready - 1; i++) {
            ready_processes[i] = ready_processes[i + 1];
        }
        // Place the current process at the end of the array
        ready_processes[*processes_ready - 1] = current_process;
    }
}

// Calculates the time at which the current process will finish its quantum
int calc_finished_time(int current_sim_time, int quantum) {

    // Check if the simulation has just started
    if (current_sim_time == 0) {
        return 0; // The finished time is 0
    } else {
        // Calculate the next multiple of quantum that is greater than or equal to the current simulation time
        int next_quantum_time = ((current_sim_time + quantum - 1) / quantum) * quantum;
        return next_quantum_time;
    }
}

// updates the finish time of the current process and clears the ready queue
void complete_process(Process *processes, Process **ready_processes, int total_processes, Process *current_process, int current_sim_time, int quantum, char* finished_process_name, int *processes_ready) {
    
// Update the finish time of the current process
    for (int i = 0; i < total_processes; i++) {
        if (strcmp(processes[i].process_name, current_process->process_name) == 0) {
            processes[i].finish_time = calc_finished_time(current_sim_time,quantum);
        }
    }
    
    // Clear the ready queue
    for (int i = 0; i < *processes_ready; i++) {
        ready_processes[i] = NULL;
    }
    *processes_ready=0;

    // Print out the finished process's information
    printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n", calc_finished_time(current_sim_time,quantum), finished_process_name, *processes_ready);

}

// updates the ready queue after a process has been scheduled
void update_ready_processes(int *processes_ready, Process **ready_processes) {
    
    // If there is more than one process in the ready queue, move each process up by one index
    if (*processes_ready > 1) {
        for (int i = 0; i < *processes_ready - 1; i++) {
            ready_processes[i] = ready_processes[i + 1];
        }
        // Decrement the number of processes ready
        (*processes_ready)--;
    }
    // If there is only one process in the ready queue, clear the ready_processes array
    else {
        ready_processes[0] = NULL;
        *processes_ready=0;
    }

}

//Sorts processes in priority order of remaining time, arrival time, and name
int sort_ready_processes(const void *p1, const void *p2) {
    const Process *process_1 = *(const Process **)p1;
    const Process *process_2 = *(const Process **)p2;

    if (process_1->remaining_time < process_2->remaining_time) {
        return -1;
    } else if (process_1->remaining_time > process_2->remaining_time) {
        return 1;
    } else {
        if (process_1->arrival_time < process_2->arrival_time) {
            return -1;
        } else if (process_1->arrival_time > process_2->arrival_time) {
            return 1;
        } else {
            return strcmp(process_1->process_name, process_2->process_name);
        }
    }
}   

// Adds processes to ready queue
void add_ready_processes(Process *processes, int total_processes, Process **ready_processes, int *processes_ready, int current_sim_time, int quantum, MemoryBlock *memory_blocks, int *num_blocks, int mem_strategy) {
    
    // iterate over all processes to find ones that are ready to run
    for (int i = *processes_ready; i < total_processes; i++) {

        // check if process has arrived and not yet completed
        if ((processes[i].arrival_time <= current_sim_time) && (processes[i].completion_time == 0)) {

            // check if process is already in ready_processes array
            int already_ready = 0;
            for (int j = 0; j < *processes_ready; j++) {
                if (strcmp(processes[i].process_name, ready_processes[j]->process_name) == 0) {
                    already_ready = 1;
                    break;
                }
            }

            // add process to ready_processes array if not already in it
            if (!already_ready) {

                // if memory allocation strategy is enabled, allocate memory to the process before adding it to the ready list
                if (mem_strategy==1) {
                    allocate_memory(num_blocks, memory_blocks, &processes[i], processes_ready, current_sim_time);
                    if (processes[i].has_memory==1) {
                        ready_processes[*processes_ready] = &processes[i];
                        (*processes_ready)++;
                    }
                }

                // if memory allocation strategy is not enabled, add process directly to the ready list
                else if (mem_strategy==0) {
                    ready_processes[*processes_ready] = &processes[i];
                    (*processes_ready)++;
                }

            }
        }
    }
}

// Calculates number of processes added to ready queue before completion of the current process
int proc_before_completion (Process **ready_processes, Process *processes, int *processes_ready, int current_sim_time, int quantum) {

    int ready_before_completion = 0;

    // Iterate through the ready queue
    for (int i = 0; i < *processes_ready; i++) {

    // If the arrival time of the process is less than current_sim_time minus the quantum value,
    // increment the ready_before_completion counter.
    if ((ready_processes[i]->arrival_time < (current_sim_time - quantum))) {
        ready_before_completion++;
    }

}
    return ready_before_completion;
}

