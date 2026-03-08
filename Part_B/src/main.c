#include <stdio.h>
#include <string.h>
#include "scheduler.h"

int main() {

    FILE *file = fopen("input.txt", "r");

    if (file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    PCB processes[100];
    int count = 0;

    while (fscanf(file, "%s %d %d %d",
        processes[count].pid,
        &processes[count].arrival_time,
        &processes[count].burst_time,
        &processes[count].priority) == 4) {

        processes[count].remaining_time = processes[count].burst_time;
        processes[count].start_time = -1;
        processes[count].completion_time = 0;
        processes[count].state = NEW;

        count++;
    }

    fclose(file);

    run_fcfs(processes, count);

    return 0;
}