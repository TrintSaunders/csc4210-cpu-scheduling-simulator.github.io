#include <stdio.h>
#include "pcb.h"
#include "scheduler.h"

const char* get_state_name(State s) {

    switch (s) {
        case NEW:
            return "NEW";
        case READY:
            return "READY";
        case RUNNING:
            return "RUNNING";
        case TERMINATED:
            return "TERMINATED";
        default:
            return "UNKNOWN";
    }
}

void run_fcfs(PCB processes[], int count) {

    int time = 0;
    int completed = 0;
    int current = -1;

    while (completed < count) {

        printf("\nTime %d:\n", time);

        // Admit new arrivals
        for (int i = 0; i < count; i++) {
            if (processes[i].arrival_time == time &&
                processes[i].state == NEW) {

                processes[i].state = READY;
            }
        }

        // If CPU idle, pick first READY (FCFS)
        if (current == -1) {
            for (int i = 0; i < count; i++) {
                if (processes[i].state == READY) {
                    current = i;
                    processes[i].state = RUNNING;

                    if (processes[i].start_time == -1) {
                        processes[i].start_time = time;
                    }

                    break;
                }
            }
        }

        // Run current process
        if (current != -1) {
            processes[current].remaining_time--;

            if (processes[current].remaining_time == 0) {
                processes[current].state = TERMINATED;
                processes[current].completion_time = time + 1;
                current = -1;
                completed++;
            }
        }

        // Debug print
        for (int i = 0; i < count; i++) {
            printf("PID=%s State=%s Remaining=%d\n",
                processes[i].pid,
                get_state_name(processes[i].state),
                processes[i].remaining_time);
        }

        time++;
    }
}