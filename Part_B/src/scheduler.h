#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"
#include <stdio.h>

/* ── utilities ─────────────────────────────────────────────── */
const char *get_state_name(State s);
int         load_processes(const char *filename, Process out[], int maxCount);
void        copy_processes(Process dst[], const Process src[], int count);

/* ── per-time-unit print helpers ───────────────────────────── */
void print_pcb_line  (FILE *f, const Process *p);
void print_running   (FILE *f, const Process *running);      /* NULL → IDLE */
void print_ready_queue_arr(FILE *f, Process procs[],
                           int queue[], int front, int rear);

/* ── Gantt chart ───────────────────────────────────────────── */
/* gantt[] is a caller-supplied array of 200 ints (-1 = idle).
   gantt[t] holds the process-array index running at time t.    */
void print_gantt(FILE *f, Process procs[], int gantt[], int totalTime);

/* ── metrics ───────────────────────────────────────────────── */
void print_metrics(FILE *f, Process procs[], int count);

/* ── scheduling algorithms ─────────────────────────────────── */
void run_fcfs      (Process procs[], int count, FILE *f);
void run_srtf      (Process procs[], int count, FILE *f);
void run_priority  (Process procs[], int count, FILE *f);
void run_round_robin(Process procs[], int count, int quantum, FILE *f);

#endif
