#include <stdio.h>
#include "scheduler.h"

const char* get_state_name(State s) {

    switch (s) {
        case NEW: return "NEW";
        case READY: return "READY";
        case RUNNING: return "RUNNING";
        case TERMINATED: return "TERMINATED";
        default: return "UNKNOWN";
    }
}

void print_ready_queue(PCB processes[], int queue[], int front, int rear) {

    printf("\nREADY:\n");

    if (front == rear) {
        printf("Empty\n");
        return;
    }

    for (int i = front; i < rear; i++) {

        int idx = queue[i];

        printf("PID=%s Arr=%d Burst=%d Rem=%d Prio=%d State=%s\n",
            processes[idx].pid,
            processes[idx].arrival_time,
            processes[idx].burst_time,
            processes[idx].remaining_time,
            processes[idx].priority,
            get_state_name(processes[idx].state));
    }
}

void run_fcfs(PCB processes[], int count) {

    int time = 0;
    int completed = 0;
    int current = -1;

    int ready_queue[100];
    int front = 0;
    int rear = 0;

    while (completed < count) {

        printf("\nTime %d:\n", time);

        /* Admit new arrivals */
        for (int i = 0; i < count; i++) {

            if (processes[i].arrival_time == time &&
                processes[i].state == NEW) {

                processes[i].state = READY;
                ready_queue[rear++] = i;
            }
        }

        /* If CPU idle, schedule next process */
        if (current == -1 && front < rear) {

            current = ready_queue[front++];
            processes[current].state = RUNNING;

            if (processes[current].start_time == -1)
                processes[current].start_time = time;
        }

        /* Run process */
        if (current != -1) {

            processes[current].remaining_time--;

            printf("\nRUNNING:\n");

            printf("PID=%s Arr=%d Burst=%d Rem=%d Prio=%d State=%s\n",
                processes[current].pid,
                processes[current].arrival_time,
                processes[current].burst_time,
                processes[current].remaining_time,
                processes[current].priority,
                get_state_name(processes[current].state));

            if (processes[current].remaining_time == 0) {

                processes[current].state = TERMINATED;
                processes[current].completion_time = time + 1;

                current = -1;
                completed++;
            }
        }

        print_ready_queue(processes, ready_queue, front, rear);

        time++;
    }
}