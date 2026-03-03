#ifndef PCB_H
#define PCB_H

typedef enum {
    NEW,
    READY,
    RUNNING,
    TERMINATED
} State;

typedef struct {
    char pid[10];          // Process ID (like "P1")
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;

    State state;

    int start_time;
    int completion_time;
} PCB;

#endif

