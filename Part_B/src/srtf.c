#include <stdio.h>
#include "scheduler.h"

/*
 * SRTF – Shortest Remaining Time First (preemptive, event-based)
 *
 * Preemption rule: at every time unit, if any READY process has
 * strictly LESS remaining time than the current RUNNING process,
 * preempt (RUNNING → READY, new shortest → RUNNING).
 *
 * Ready queue is kept as an unordered array; we scan for the
 * shortest each time.  This mirrors a real OS priority queue
 * without using STL containers.
 */
void run_srtf(Process procs[], int count, FILE *f) {

    fprintf(f, "========================================\n");
    fprintf(f, "        SRTF Scheduling\n");
    fprintf(f, "========================================\n\n");

    /*
     * ready[] holds indices into procs[] for all READY processes.
     * readyCount is the current occupancy.
     * We maintain insertion order for printing, and scan for min.
     */
    int ready[200];
    int readyCount = 0;

    int gantt[500];
    int totalTime = 0;

    int current   = -1;
    int completed = 0;
    int time      = 0;

    while (completed < count) {

        /* ── 1. Admit arrivals ── */
        for (int i = 0; i < count; i++) {
            if (procs[i].arrival == time && procs[i].state == NEW) {
                procs[i].state     = READY;
                ready[readyCount++] = i;
            }
        }

        /* ── 2. Event-based preemption check ──
               If there is a running process AND some ready process
               has strictly shorter remaining time, preempt.         */
        if (current != -1 && readyCount > 0) {
            /* find the ready process with smallest remaining time */
            int minIdx  = 0;
            for (int i = 1; i < readyCount; i++) {
                if (procs[ready[i]].remaining < procs[ready[minIdx]].remaining)
                    minIdx = i;
            }

            if (procs[ready[minIdx]].remaining < procs[current].remaining) {
                /* Preempt: push current back to ready */
                procs[current].state = READY;
                ready[readyCount++]  = current;

                /* Pull the shortest ready process */
                current = ready[minIdx];
                /* Remove minIdx from ready[] by shifting */
                for (int i = minIdx; i < readyCount - 1; i++)
                    ready[i] = ready[i + 1];
                readyCount--;

                procs[current].state = RUNNING;
                if (procs[current].startTime == -1)
                    procs[current].startTime = time;
            }
        }

        /* ── 3. If CPU idle, pick shortest ready process ── */
        if (current == -1 && readyCount > 0) {
            int minIdx = 0;
            for (int i = 1; i < readyCount; i++) {
                if (procs[ready[i]].remaining < procs[ready[minIdx]].remaining)
                    minIdx = i;
            }
            current = ready[minIdx];
            for (int i = minIdx; i < readyCount - 1; i++)
                ready[i] = ready[i + 1];
            readyCount--;

            procs[current].state = RUNNING;
            if (procs[current].startTime == -1)
                procs[current].startTime = time;
        }

        /* ── 4. Print snapshot ── */
        fprintf(f, "Time %d:\n", time);
        print_running(f, current == -1 ? NULL : &procs[current]);

        /* Print ready queue in current order */
        fprintf(f, "READY:\n");
        for (int i = 0; i < readyCount; i++)
            print_pcb_line(f, &procs[ready[i]]);
        fprintf(f, "\n");

        /* ── 5. Record Gantt ── */
        gantt[time] = current;
        totalTime   = time + 1;

        /* ── 6. Advance running process ── */
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
