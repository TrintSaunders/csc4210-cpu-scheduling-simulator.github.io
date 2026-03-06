#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcb.h"
#include "scheduler.h"

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
    run_fcfs(processes, count);

    return 0;
}
