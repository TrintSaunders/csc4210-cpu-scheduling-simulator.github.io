#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcb.h"

#define MAX_PROCESSES 100

int main() {
    FILE *file;
    PCB processes[MAX_PROCESSES];
    int count = 0;

    file = fopen("input.txt", "r");

    if (file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    while (fscanf(file, "%s %d %d %d",
                  processes[count].pid,
                  &processes[count].arrival_time,
                  &processes[count].burst_time,
                  &processes[count].priority) == 4) {

        processes[count].remaining_time = processes[count].burst_time;
        processes[count].state = NEW;
        processes[count].start_time = -1;
        processes[count].completion_time = -1;

        count++;
    }

    fclose(file);

     printf("Loaded %d processes:\n\n", count);

    for (int i = 0; i < count; i++) {
        printf("PID=%s Arrival=%d Burst=%d Priority=%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority);
    }

    int time = 0;
    int completed = 0;
    int current = -1;  // index of currently running process

    while (completed < count) {

        printf("\nTime %d:\n", time);

        // 1. Admit new arrivals
        for (int i = 0; i < count; i++) {
            if (processes[i].arrival_time == time &&
                processes[i].state == NEW) {

                processes[i].state = READY;
            }
        }

        // 2. If CPU is idle, pick next READY (FCFS logic for now)
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

        // 3. Run current process
        if (current != -1) {
            processes[current].remaining_time--;

            if (processes[current].remaining_time == 0) {
                processes[current].state = TERMINATED;
                processes[current].completion_time = time + 1;
                current = -1;
                completed++;
            }
        }

        // 4. Print states
        for (int i = 0; i < count; i++) {
            printf("PID=%s State=%d Remaining=%d\n",
                processes[i].pid,
                processes[i].state,
                processes[i].remaining_time);
        }

        time++;
    }

    return 0;
}
