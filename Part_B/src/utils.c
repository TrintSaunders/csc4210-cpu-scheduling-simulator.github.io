#include <stdio.h>
#include <string.h>
#include "scheduler.h"

/* ────────────────────────────────────────────────────────────
   State name
   ──────────────────────────────────────────────────────────── */
const char *get_state_name(State s) {
    switch (s) {
        case NEW:        return "NEW";
        case READY:      return "READY";
        case RUNNING:    return "RUNNING";
        case TERMINATED: return "TERMINATED";
        default:         return "UNKNOWN";
    }
}

/* ────────────────────────────────────────────────────────────
   Load processes from file.
   Skips the optional header line if it starts with 'P' or 'p'
   followed by a non-digit (i.e. the word "PID").
   Returns number of processes read.
   ──────────────────────────────────────────────────────────── */
int load_processes(const char *filename, Process out[], int maxCount) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Error: cannot open %s\n", filename); return 0; }

    int i = 0;
    char line[128];

    while (i < maxCount && fgets(line, sizeof(line), f)) {
        /* skip blank lines and header */
        if (line[0] == '\n' || line[0] == '\r') continue;
        if (line[0] == 'P' && (line[1] < '0' || line[1] > '9')) continue;

        if (sscanf(line, "%9s %d %d %d",
                   out[i].pid,
                   &out[i].arrival,
                   &out[i].burst,
                   &out[i].priority) == 4)
        {
            out[i].remaining     = out[i].burst;
            out[i].state         = NEW;
            out[i].startTime     = -1;
            out[i].completionTime= -1;
            i++;
        }
    }

    fclose(f);
    return i;
}

/* ────────────────────────────────────────────────────────────
   Deep-copy process array (so each algorithm gets a fresh copy)
   ──────────────────────────────────────────────────────────── */
void copy_processes(Process dst[], const Process src[], int count) {
    for (int i = 0; i < count; i++) dst[i] = src[i];
}

/* ────────────────────────────────────────────────────────────
   Print a single PCB line (matches spec format)
   ──────────────────────────────────────────────────────────── */
void print_pcb_line(FILE *f, const Process *p) {
    fprintf(f, "  PID=%s Arr=%d Burst=%d Rem=%d Prio=%d State=%s\n",
            p->pid, p->arrival, p->burst,
            p->remaining, p->priority,
            get_state_name(p->state));
}

/* ────────────────────────────────────────────────────────────
   Print RUNNING block.  Pass NULL for idle CPU.
   ──────────────────────────────────────────────────────────── */
void print_running(FILE *f, const Process *running) {
    fprintf(f, "RUNNING:\n");
    if (!running)
        fprintf(f, "  IDLE\n");
    else
        print_pcb_line(f, running);
}

/* ────────────────────────────────────────────────────────────
   Print READY queue (circular-array version used by FCFS / RR /
   Priority).  front..rear are indices into queue[].
   ──────────────────────────────────────────────────────────── */
void print_ready_queue_arr(FILE *f, Process procs[],
                           int queue[], int front, int rear) {
    fprintf(f, "READY:\n");
    for (int i = front; i < rear; i++)
        print_pcb_line(f, &procs[queue[i]]);
}

/* ────────────────────────────────────────────────────────────
   Gantt chart  (text-art version)

   gantt[t] = process-array index running at time t, or -1 (idle).
   ──────────────────────────────────────────────────────────── */
void print_gantt(FILE *f, Process procs[], int gantt[], int totalTime) {
    fprintf(f, "\n=== Gantt Chart ===\n");

    /* ── top border ── */
    fprintf(f, "+");
    for (int t = 0; t < totalTime; t++) fprintf(f, "----+");
    fprintf(f, "\n|");

    /* ── process labels ── */
    for (int t = 0; t < totalTime; t++) {
        if (gantt[t] == -1)
            fprintf(f, "IDLE|");
        else
            fprintf(f, " %2s |", procs[gantt[t]].pid);
    }
    fprintf(f, "\n+");

    /* ── bottom border ── */
    for (int t = 0; t < totalTime; t++) fprintf(f, "----+");

    /* ── time markers ── */
    fprintf(f, "\n0");
    for (int t = 0; t < totalTime; t++) fprintf(f, "    %d", t + 1);
    fprintf(f, "\n");
}

/* ────────────────────────────────────────────────────────────
   Per-process metrics + averages
   ──────────────────────────────────────────────────────────── */
void print_metrics(FILE *f, Process procs[], int count) {
    fprintf(f, "\n=== Scheduling Metrics ===\n");
    fprintf(f, "%-6s %-10s %-13s %-15s %-15s\n",
            "PID", "Arrival", "Burst", "Turnaround", "Waiting");

    double sumTAT = 0, sumWT = 0, sumRT = 0;

    for (int i = 0; i < count; i++) {
        int tat = procs[i].completionTime - procs[i].arrival;
        int wt  = tat - procs[i].burst;
        int rt  = procs[i].startTime - procs[i].arrival;

        fprintf(f, "%-6s %-10d %-13d %-15d %-15d\n",
                procs[i].pid, procs[i].arrival,
                procs[i].burst, tat, wt);

        sumTAT += tat;
        sumWT  += wt;
        sumRT  += rt;
    }

    fprintf(f, "\nAverage Turnaround Time : %.2f\n", sumTAT / count);
    fprintf(f, "Average Waiting Time    : %.2f\n",  sumWT  / count);
    fprintf(f, "Average Response Time   : %.2f\n",  sumRT  / count);
}
