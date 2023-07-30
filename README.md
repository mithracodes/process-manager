# process-scheduling-manager

## Overview

The process manager is capable of allocating memory to processes and scheduling them for execution. The manager consists of two phases: in the first phase, a scheduling algorithm will allocate the CPU to processes, assuming memory requirements are always satisfied. In the second phase, a memory allocation algorithm will be used to allocate memory to processes before scheduling takes place. The memory allocation and process scheduling will be simulated. The process scheduling decisions will be based on a scheduling algorithm that assumes a single CPU, meaning only one process can run at a time.

## The Process Manager

The process manager runs in cycles, where each cycle occurs after one quantum has elapsed. The simulation time starts at 0 and increases by the length of the quantum every cycle. Thus, at any given cycle, the simulation time (TS) is equal to the number of completed cycles times the length of the quantum:

**T<sub>S</sub> = N * Q** 

Where T<sub>S</sub> is the current simulation time, N is the number of cycles that have elapsed, and Q is the length of the quantum. Q is an integer value between 1 and 3 (1 ≤ Q ≤ 3).

At the start of each cycle, the process manager carries the following tasks in sequence:

1. Identify whether the currently running process (if any) has completed. If so, it is terminated, and its memory deallocated before subsequent scheduling tasks are performed.

2. Identify all processes that have been submitted to the system since the last cycle occurred and add them to the input queue in the order they appear in the process file. A process is considered to have been submitted to the system if its arrival time is less than or equal to the current simulation time T<sub>s</sub>.

3. The input queue contains processes that have been submitted to the system and are waiting to be brought into memory (from disk) to compete for CPU time. At no time should the input queue contain processes whose arrival time is larger than the current simulation time.

4. Move processes from the input queue to the ready queue upon successful memory allocation. The ready queue holds processes that are ready to run.

The process manager uses ONE of the following methods to allocate memory to processes:

- **Method 1:** Assume that the memory requirements of the arrived processes are always immediately satisfied (e.g., there is an infinite amount of memory). All the arrived processes will automatically enter a READY state and be placed at the end of the ready queue in the same order as they appear in the input queue.

* **Method 2:** Allocate memory to processes in the input queue according to a memory allocation algorithm. Processes for which memory allocation is successful enter a READY state and are placed at the end of the ready queue in order of memory allocation to await CPU time.

  Determine the process (if any) which runs in this cycle. Depending on the scheduling   algorithm, this could be the process that was previously running, a resumed process that was previously placed back into the ready queue, or a READY process which has not been previously executed.
  
After completing these tasks, the process manager sleeps for Q seconds, after which a new cycle begins.

## Programming Tasks

### Task 1: Process Scheduling (Non-preemptive)

**Shortest Job First (SJF)**

The process with the shortest service time is chosen among all the READY processes to run. The process will be allowed to run for the entire duration of its service time without getting suspended or terminated (i.e., non-preemptive). If there is a tie when choosing the shortest process (i.e., two or more processes have the same service time), the process with the earlier arrival time is selected. In case there are two or more processes with the same service time and arrival time, the process whose name comes first lexicographically will be chosen.

In the case of SJF, after one quantum has elapsed:

- If the process has completed its execution, it will be terminated and moved to the FINISHED state.
* If the process requires more CPU time, it will continue to run for another quantum.

### Task 2: Process Scheduling (Pre-emptive)

**Round Robin (RR)** 

The process at the head of the READY queue is chosen to run for one quantum. After a process has run for one quantum, it is suspended and placed at the tail of the ready queue unless there are no other processes currently in the ready queue.

In the case of RR, after one quantum has elapsed:

- If the process has completed its execution, it will be terminated and moved to the FINISHED state.
* If the process requires more CPU time and there are other READY processes, the process will be suspended and enters the READY state again to await more CPU time.
+ If the process requires more CPU time and there are no other READY processes, the process can continue to run for another quantum.

### Task 3: Memory Allocation

In this task, an additional memory allocation phase is implemented before the program starts the scheduling phase of the simulation. Note that memory allocation is also simulated, i.e. the process manager only has to track and report simulated memory addresses.

In each process management cycle (i.e., after each quantum), the program allocates memory to all the processes in the input queue, serving processes in the order of appearance (i.e., from the first process in the input queue to the last), and moves successfully allocated processes to the ready queue in order of allocation. Processes for which memory allocation cannot be currently met remains in the input queue.

Processes whose memory allocation has succeeded are considered READY to run. The input queue does not contain any READY processes.

When allocating memory to a particular process, the Best Fit memory allocation algorithm is implemented.

## Running the program

Usage: ```allocate -f <filename> -s (SJF | RR) -m (infinite | best-fit) -q (1 | 2 | 3)```

```-f``` filename will specify a valid relative or absolute path to the input file describing the processes.
```-s``` scheduler where scheduler is one of *{SJF, RR}*.
```-m``` memory-strategy where memory-strategy is one of *{infinite, best-fit}*.
```-q``` quantum where quantum is one of *{1, 2, 3}*.

The filename contains the list of processes to be executed, with each line containing a process.
Each process is represented by a space-separated tuple (time-arrived, process-name, service-time, memory-requirement).

It is assumed that:

- The file will be sorted by time-arrived which is an integer in [0, 232) indicating seconds
* All process-names will be distinct uppercase alphanumeric strings of minimum length 1 and
maximum length 8.
+ The frst process will always have time-arrived set to 0.
- service-time will be an integer in [1, 232) indicating seconds.
* memory-requirement will be an integer in [1, 2048] indicating MBs of memory required.
+ The fle is space delimited, and each line (including the last) will be terminated with an LF
(ASCII 0x0a) control character.


*** Note: This is my submission for *Project 1 of COMP30023 Computer Systems in Sem 1 2023*. ***
