#include <stdio.h>
#include "scheduler.h"

/*
 * Round Robin – preemptive, time-based
 *
 * Preemption rule: when quantum_used == quantum AND process is not
 * finished, move RUNNING → READY (append to tail of queue).
 *
 * Arrival tie-break with quantum expiry: new arrivals at the same
 * tick that a quantum expires are enqueued BEFORE the preempted
 * process (they arrived this tick, preempted process goes to back).
 */
void run_round_robin(Process procs[], int count, int quantum, FILE *f) {

    fprintf(f, "========================================\n");
    fprintf(f, "        Round Robin Scheduling  (quantum=%d)\n", quantum);
    fprintf(f, "========================================\n\n");

    /*
     * Circular-style array queue used as a simple FIFO.
     * front/rear are ever-increasing indices (no wrap-around needed
     * because max queue size is count * (max_burst / quantum) + 1,
     * well within 1000).
     */
    int queue[1000];
    int front = 0, rear = 0;

    int gantt[500];
    int totalTime = 0;

    int current      = -1;
    int quantum_used =  0;
    int completed    =  0;
    int time         =  0;

    while (completed < count) {

        /* ── 1. Handle quantum expiry BEFORE admitting new arrivals
               so new arrivals get enqueued first (they go ahead of
               the preempted process per standard RR behaviour).   ──*/
        int preempted = 0;
        if (current != -1 && quantum_used == quantum) {
            procs[current].state = READY;
            preempted            = 1;   /* defer re-enqueue until after arrivals */
            /* do NOT reset current yet */
        }

        /* ── 2. Admit arrivals ── */
        for (int i = 0; i < count; i++) {
            if (procs[i].arrival == time && procs[i].state == NEW) {
                procs[i].state  = READY;
                queue[rear++]   = i;
            }
        }

        /* ── 3. Enqueue preempted process AFTER new arrivals ── */
        if (preempted) {
            queue[rear++] = current;
            current       = -1;
            quantum_used  = 0;
        }

        /* ── 4. Select next process if CPU is idle ── */
        if (current == -1 && front < rear) {
            current = queue[front++];
            procs[current].state = RUNNING;
            quantum_used         = 0;
            if (procs[current].startTime == -1)
                procs[current].startTime = time;
        }

        /* ── 5. Print snapshot ── */
        fprintf(f, "Time %d:\n", time);
        print_running(f, current == -1 ? NULL : &procs[current]);
        print_ready_queue_arr(f, procs, queue, front, rear);
        fprintf(f, "\n");

        /* ── 6. Record Gantt ── */
        gantt[time] = current;
        totalTime   = time + 1;

        /* ── 7. Advance running process ── */
        if (current != -1) {
            procs[current].remaining--;
            quantum_used++;

            if (procs[current].remaining == 0) {
                procs[current].state          = TERMINATED;
                procs[current].completionTime = time + 1;
                current      = -1;
                quantum_used = 0;
                completed++;
            }
        }

        time++;
    }

    print_gantt(f, procs, gantt, totalTime);
    print_metrics(f, procs, count);
}
