
#include <stdlib.h>
#include "scheduling.h"

//Parses through command line arguments
void parse_arguments(char** argv, int* scheduler, int* mem_strategy, int* quantum, FILE** fp) {
    
    //if the scheduling algorithm is SJF
    if (strcmp(argv[4], "SJF") == 0) {
        *scheduler = 0;

    //if the scheduling algorithm is RR
    } else if (strcmp(argv[4], "RR") == 0) {
        *scheduler = 1;
    
    //print error message
    } else {
        printf("Invalid scheduling algorithm\n");
        exit(1);
    }

    // Check if the memory allocation strategy is infinite
    if (strcmp(argv[6], "infinite") == 0) {
        *mem_strategy = 0;
    } 

    // Check if the memory allocation strategy is best-fit
    else if (strcmp(argv[6], "best-fit") == 0) {
        *mem_strategy = 1;

    // print error message
    } else {
        printf("Invalid memory allocation strategy.\n");
        exit(1);
    }

    // Check if the quantum is a valid integer (1, 2, or 3)
    if (strcmp(argv[8], "1") == 0 || strcmp(argv[8], "2") == 0 || strcmp(argv[8], "3") == 0) {
        *quantum = atoi(argv[8]);
    
    // error message
    } else {
        printf("Invalid quantum time.\n");
        exit(1);
    }

    // Open the input file
    *fp = fopen(argv[2], "r");
    if (*fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
}

// Prints performance metrics
void print_metrics(Process *processes, int total_processes, int quantum, int current_sim_time) {
    
    // Calculate and print average turnaround time
    int total_turnaround_time = 0;
    for (int i = 0; i < total_processes; i++) {
        total_turnaround_time += processes[i].finish_time - processes[i].arrival_time;
    }
    int avg_turnaround_time = (total_turnaround_time + total_processes-1) / total_processes;
    printf("Turnaround time %d\n", avg_turnaround_time);

    // Calculate and print maximum and average time overhead
    float max_time_overhead = 0.0;
    float avg_time_overhead = 0.0;

    for (int i = 0; i < total_processes; i++) {
        int turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        int service_time = processes[i].simulation_time;

        float overhead = (float) turnaround_time / (float) service_time;
        if (overhead > max_time_overhead) {
            max_time_overhead = overhead;
        }
        avg_time_overhead += overhead;
    }
    avg_time_overhead /= total_processes;
    printf("Time overhead %.2f %.2f\n", max_time_overhead, avg_time_overhead);

    // Calculate and print makespan
    printf("Makespan %d\n", calc_finished_time(current_sim_time,quantum));
}