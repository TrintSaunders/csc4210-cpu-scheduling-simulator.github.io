#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "pcb.h"

void run_fcfs(PCB processes[], int count);
void run_round_robin(PCB processes[], int count, int quantum);
const char* get_state_name(State s);

#endif