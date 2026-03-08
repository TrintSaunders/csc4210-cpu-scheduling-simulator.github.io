#include <stdio.h>
#include "scheduler.h"

/*
 * FCFS – First Come First Served (non-preemptive)
 *
 * Selection rule: process that arrived earliest.
 * Ties broken by array order (which mirrors input order).
 */
void run_fcfs(Process procs[], int count, FILE *f) {

    fprintf(f, "========================================\n");
    fprintf(f, "        FCFS Scheduling\n");
    fprintf(f, "========================================\n\n");

    /* Simple array-based queue (indices into procs[]) */
    int queue[200];
    int front = 0, rear = 0;

    /* gantt[t] = procs index running at time t, -1 = idle */
    int gantt[500];
    int totalTime = 0;

    int current   = -1;   /* index of running process, -1 = none */
    int completed = 0;
    int time      = 0;

    /* Find the latest completion time to bound the loop */
    /* We run until all processes complete */
    while (completed < count) {

        /* ── 1. Admit newly arrived processes (NEW → READY) ── */
        for (int i = 0; i < count; i++) {
            if (procs[i].arrival == time && procs[i].state == NEW) {
                procs[i].state = READY;
                queue[rear++]  = i;
            }
        }

        /* ── 2. Select next process if CPU is idle ── */
        if (current == -1 && front < rear) {
            current = queue[front++];
            procs[current].state = RUNNING;
            if (procs[current].startTime == -1)
                procs[current].startTime = time;
        }

        /* ── 3. Print time-unit snapshot ── */
        fprintf(f, "Time %d:\n", time);
        print_running(f, current == -1 ? NULL : &procs[current]);
        print_ready_queue_arr(f, procs, queue, front, rear);
        fprintf(f, "\n");

        /* ── 4. Record Gantt slot ── */
        gantt[time] = current;
        totalTime   = time + 1;

        /* ── 5. Advance running process ── */
        if (current != -1) {
            procs[current].remaining--;

            if (procs[current].remaining == 0) {
                procs[current].state          = TERMINATED;
                procs[current].completionTime = time + 1;
                current = -1;
                completed++;
            }
        }

        time++;
    }

    print_gantt(f, procs, gantt, totalTime);
    print_metrics(f, procs, count);
}
