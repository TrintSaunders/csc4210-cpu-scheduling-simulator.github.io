#include <stdio.h>
#include "scheduler.h"

/*
 * Priority – Non-preemptive
 *
 * Selection rule: when the CPU becomes free, pick the READY process
 * with the LOWEST priority number (1 = highest urgency).
 * Ties broken by earliest arrival, then by input order.
 *
 * Non-preemptive: once a process starts, it runs to completion.
 */
void run_priority(Process procs[], int count, FILE *f) {

    fprintf(f, "========================================\n");
    fprintf(f, "        Priority Scheduling (Non-Preemptive)\n");
    fprintf(f, "========================================\n\n");

    int queue[200];
    int queueCount = 0;   /* unordered list of READY indices */

    int gantt[500];
    int totalTime = 0;

    int current   = -1;
    int completed = 0;
    int time      = 0;

    while (completed < count) {

        /* ── 1. Admit arrivals ── */
        for (int i = 0; i < count; i++) {
            if (procs[i].arrival == time && procs[i].state == NEW) {
                procs[i].state      = READY;
                queue[queueCount++] = i;
            }
        }

        /* ── 2. Select next process when CPU is free ── */
        if (current == -1 && queueCount > 0) {
            /*
             * Find lowest priority number among all READY.
             * Tie-break: earliest arrival → then input order.
             */
            int best = 0;
            for (int i = 1; i < queueCount; i++) {
                int bi = queue[best];
                int ci = queue[i];
                if (procs[ci].priority < procs[bi].priority)
                    best = i;
                else if (procs[ci].priority == procs[bi].priority &&
                         procs[ci].arrival  <  procs[bi].arrival)
                    best = i;
            }
            current = queue[best];
            /* Remove from queue by shifting */
            for (int i = best; i < queueCount - 1; i++)
                queue[i] = queue[i + 1];
            queueCount--;

            procs[current].state = RUNNING;
            if (procs[current].startTime == -1)
                procs[current].startTime = time;
        }

        /* ── 3. Print snapshot ── */
        fprintf(f, "Time %d:\n", time);
        print_running(f, current == -1 ? NULL : &procs[current]);

        fprintf(f, "READY:\n");
        for (int i = 0; i < queueCount; i++)
            print_pcb_line(f, &procs[queue[i]]);
        fprintf(f, "\n");

        /* ── 4. Record Gantt ── */
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
