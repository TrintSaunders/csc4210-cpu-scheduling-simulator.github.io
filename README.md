# csc4210-cpu-scheduling-simulator.github.io

Compilation:
gcc src/main.c src/scheduler.c src/pcb.c -o scheduler

Run:
./scheduler

Assumptions:
- Time is simulated in discrete integer units.
- Lower priority number means higher priority.