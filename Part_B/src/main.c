#include <stdio.h>
#include "scheduler.h"

#define MAX_PROCESSES 100

int main(void) {

    Process original[MAX_PROCESSES];
    Process procs[MAX_PROCESSES];

    /* ── Load input file ── */
    int count = load_processes("input.txt", original, MAX_PROCESSES);
    if (count == 0) {
        printf("No processes loaded. Check input.txt\n");
        return 1;
    }

    printf("Loaded %d processes from input.txt\n", count);

    /* ── Read quantum for Round Robin ── */
    int quantum;
    printf("Enter time quantum for Round Robin: ");
    scanf("%d", &quantum);

    /* ──────────────────────────────────────────
       FCFS
       ────────────────────────────────────────── */
    copy_processes(procs, original, count);
    FILE *f = fopen("output_fcfs.txt", "w");
    if (!f) { perror("output_fcfs.txt"); return 1; }
    run_fcfs(procs, count, f);
    fclose(f);
    printf("FCFS  done  → output_fcfs.txt\n");

    /* ──────────────────────────────────────────
       SRTF
       ────────────────────────────────────────── */
    copy_processes(procs, original, count);
    f = fopen("output_srtf.txt", "w");
    if (!f) { perror("output_srtf.txt"); return 1; }
    run_srtf(procs, count, f);
    fclose(f);
    printf("SRTF  done  → output_srtf.txt\n");

    /* ──────────────────────────────────────────
       Priority (non-preemptive)
       ────────────────────────────────────────── */
    copy_processes(procs, original, count);
    f = fopen("output_priority.txt", "w");
    if (!f) { perror("output_priority.txt"); return 1; }
    run_priority(procs, count, f);
    fclose(f);
    printf("Prio  done  → output_priority.txt\n");

    /* ──────────────────────────────────────────
       Round Robin
       ────────────────────────────────────────── */
    copy_processes(procs, original, count);
    f = fopen("output_rr.txt", "w");
    if (!f) { perror("output_rr.txt"); return 1; }
    run_round_robin(procs, count, quantum, f);
    fclose(f);
    printf("RR    done  → output_rr.txt\n");

    return 0;
}
