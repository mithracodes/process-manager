#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "scheduling.h"



int main(int argc, char** argv) {

    // initialize variables to store user input and process information
    int scheduler = -1;
    int mem_strategy = -1;
    int quantum = -1;
    int total_processes=0;
    Process *processes = NULL;
    MemoryBlock memory_blocks[1] = {{0, 2048}}; //initialize one memory block of size 2048
    int num_blocks = 1;

    //parse command line arguments and open input file
    FILE* fp;
    parse_arguments(argv, &scheduler, &mem_strategy, &quantum, &fp);

    //read in process information from file and create a Process struct for each process
    int arrival_time, simulation_time, memory_req;
    char* process_name = NULL;
    while (fscanf(fp, "%d %ms %d %d", &arrival_time, &process_name, &simulation_time, &memory_req) == 4) {
        //allocate memory for new Process struct and add it to array of processes
        total_processes++;
        processes = realloc(processes, total_processes * sizeof(Process));
        processes[total_processes-1].arrival_time = arrival_time;
        processes[total_processes-1].process_name = process_name;
        processes[total_processes-1].simulation_time = simulation_time;
        processes[total_processes-1].memory_req = memory_req;
        processes[total_processes-1].remaining_time = simulation_time;
        processes[total_processes-1].completion_time = 0;
        processes[total_processes-1].has_memory=0;
        processes[total_processes-1].finish_time=0;

        //set process_name pointer to NULL to avoid double free
        process_name = NULL;   
    }

    //close input file
    fclose(fp);

    //initialize variables for simulation loop
    int current_sim_time = 0;
    int completed_processes = 0;
    Process *ready_processes[total_processes];
    int processes_ready = 0;
    int ready_before_completion = 0;
    char* prev_process = NULL;
 
    //execute the simulation loop until all processes have completed
    while (completed_processes < total_processes) {

        //if the process manager has not started or if the scheduler is not SJF
        if (current_sim_time<=0 || scheduler!=1) {
            add_ready_processes(processes, total_processes, ready_processes, &processes_ready, current_sim_time, quantum, memory_blocks,&num_blocks,mem_strategy);
        }

        if (processes_ready > 0) { //if there are processes in the ready queue

            if (scheduler == 0) { //if the scheduler is of type 0 (SJF)
                qsort(ready_processes, processes_ready, sizeof(Process *), sort_ready_processes); //sort the ready queue in increasing order of remaining time using quicksort algorithm          
            }

            Process *current_process = ready_processes[0]; //get the process with the shortest remaining time

            if (prev_process == NULL || strcmp(prev_process,current_process->process_name) != 0) { //if the previous process name is null or different from the current process name
                printf("%d,RUNNING,process_name=%s,remaining_time=%d\n", calc_finished_time(current_sim_time,quantum), current_process->process_name, current_process->remaining_time); //print the current time, status (RUNNING), process name and remaining time
            }
            prev_process = current_process->process_name; //set the previous process name to the current process name

            if (scheduler == 0 && mem_strategy==0) { //if the scheduler is of type 0 (SJF) and the memory allocation strategy is of type 0 (FF)
                current_sim_time += current_process->simulation_time; //update the simulation time by adding the process simulation time
                current_process->remaining_time -=  current_process->simulation_time; //update the remaining time of the current process
            } else { //if the scheduler is of type 1 (RR)
                current_sim_time += quantum; //update the simulation time by adding the quantum
                current_process-> remaining_time -= quantum; //update the remaining time of the current process
            }
        

            // If the current process terminates
            if (current_process->remaining_time <= 0) { //if process finishes

                // Store the finished process name
                char* finished_process_name = NULL;
                finished_process_name=current_process->process_name;

                // Mark the process as completed
                current_process->completion_time = 1;
                completed_processes++;

                if (completed_processes!=total_processes) {

                    // If there are more processes to execute, update the ready queue and add new ready processes
                    update_ready_processes(&processes_ready,ready_processes);
                    add_ready_processes(processes, total_processes, ready_processes, &processes_ready, current_sim_time, quantum,memory_blocks,&num_blocks,mem_strategy);
                   
                } else {
                    
                    // If this was the last process, complete it and print metrics
                    complete_process(processes,ready_processes,total_processes,current_process,current_sim_time,quantum,finished_process_name,&processes_ready);
                                        
                    int memory_address = current_process->memory_address;
                    int memory_size = current_process->memory_size;

                    if (mem_strategy==1) {
                        free_memory_block(&num_blocks,memory_blocks,memory_address,memory_size);

                    }

                    print_metrics(processes, total_processes, quantum, current_sim_time);

                    // Exit the program
                    exit(EXIT_SUCCESS);
                }
                
                // Calculate the time when the process finishes and count ready processes before completion
                int memory_address = current_process->memory_address;
                int memory_size = current_process->memory_size;
                current_sim_time = calc_finished_time(current_sim_time,quantum);
                ready_before_completion = proc_before_completion(ready_processes, processes, &processes_ready, current_sim_time, quantum);
                
                // Print finished process details and update finish time for the completed process
                printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n", current_sim_time, finished_process_name, ready_before_completion);
                
                for (int i = 0; i < total_processes; i++) {
                            //printf("process[%d]=%s ",i)
                            if (strcmp(processes[i].process_name, current_process->process_name) == 0) {
                                processes[i].finish_time = calc_finished_time(current_sim_time,quantum);
                                //printf("process no %d: %sis done\n", i+1, processes[i].process_name);

                            }
                }
                
                // Free the memory block if memory strategy is best-fit and add new ready processes
                if (mem_strategy==1) {
                    free_memory_block(&num_blocks,memory_blocks,memory_address,memory_size);
                    add_ready_processes(processes, total_processes, ready_processes, &processes_ready, current_sim_time, quantum,memory_blocks,&num_blocks,mem_strategy);
                } 

            } else {

                // If the process is not finished, add new ready processes and switch to the next process using round-robin scheduling
                add_ready_processes(processes, total_processes, ready_processes, &processes_ready, current_sim_time, quantum,memory_blocks,&num_blocks,mem_strategy);
                round_robin_scheduling(ready_processes, &processes_ready, current_process);             

            }
     
        } else {

            // wait for another quantum for a process to arrive
            current_sim_time += quantum;
        }

        
    }

    



    
    free(processes);


    return 1;
}

