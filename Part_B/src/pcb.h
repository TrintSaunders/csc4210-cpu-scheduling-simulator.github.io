#ifndef PCB_H
#define PCB_H

typedef enum {
    NEW,
    READY,
    RUNNING,
    TERMINATED
} State;

typedef struct {
    char pid[10];
    int  arrival;
    int  burst;
    int  remaining;
    int  priority;
    State state;
    int  startTime;       /* first time process hits CPU, -1 until then */
    int  completionTime;  /* time when remaining hits 0                 */
} Process;

#endif
