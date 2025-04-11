In lecture we discussed the use of various synchronization techniques

Critical section is used to make sure that other threads cannot enter this block so there is no race condition

Atomic operations are for single instructions and their use case is when we want a light weight operation on something that is shared and can involve race condition

Barrier involves specifying a place where all threads must meet and wait together until every single thread reaches that pool.

flush is used to update the value of a variable directly in the memory so there is no longer a case of cache involved while both reading and writing
