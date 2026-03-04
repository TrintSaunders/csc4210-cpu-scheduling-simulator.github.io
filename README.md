# csc4210-cpu-scheduling-simulator.github.io

CPU Scheduling Simulator
CSC 4210 – Operating Systems

----------------------------------------
Project Description
----------------------------------------
This program implements a discrete-time CPU scheduling simulator in C.

The simulator models:
• Process Control Blocks (PCBs)
• OS process states (NEW, READY, RUNNING, TERMINATED)
• Full PCB trace printed at every time unit
• Four scheduling algorithms:
    - FCFS (First Come First Serve)
    - SRTF (Shortest Remaining Time First)
    - Priority (Non-preemptive)
    - Round Robin (Preemptive, time-based)

The simulation explicitly advances one time unit at a time and prints
the complete PCB state for the RUNNING process and all READY processes
at each time step.

----------------------------------------
Compilation Instructions
----------------------------------------
From inside the Part_B directory:

gcc src/*.c -o scheduler

----------------------------------------
Run Instructions
----------------------------------------
./scheduler

----------------------------------------
Input File Format
----------------------------------------
The input file must follow this format:

PID Arrival Burst Priority

Example:

P1 0 5 2
P2 1 3 1
P3 2 1 3
P4 3 2 2

----------------------------------------
Assumptions
----------------------------------------
• Time is simulated in discrete integer units.
• Lower priority number indicates higher priority.
• Only one process may be in the RUNNING state at a time.
• Preemption rules strictly follow project specifications:
    - SRTF: event-based preemption when a READY process has
      strictly smaller remaining time.
    - Round Robin: time-based preemption when time quantum expires.
• READY queue order is preserved according to scheduling policy.

----------------------------------------
Author
----------------------------------------
Name: Trint Saunders
Course: CSC 4210
Semester: Spring 2026