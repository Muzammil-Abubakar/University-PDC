We have studied the use of nowait and how for loops work in omp and how to compile the program and all similar things.

We then also discovered where the implicit barrier is applied and where is it not. Example: There is implicit barrier between pragma for but not between standard statements.
So 2 consecutive statements can be executed by 1 thread without waiting for all other threads.