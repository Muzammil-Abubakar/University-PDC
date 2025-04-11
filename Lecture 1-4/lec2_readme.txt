We saw the use of private variables and also how each scheduling is different from the others, the comments may not reflect the code provided so dont use that as a guide for anything.

these are some types of data_sharing attributes

shared(var)	        All threads share the same variable (default for global vars).
private(var)	    Each thread gets its own uninitialized copy.
firstprivate(var)	Like private, but initialized with the value from before the parallel region.
lastprivate(var)	Like private, but last iteration's value is copied back to the original variable after the loop.
default(shared)	    Set the default behavior of unspecified variables.
default(none)	    Forces you to explicitly declare the sharing attribute of each variable (recommended for safety).
reduction(op: var)	Combines all threads' results using a specified operation (like +, *, etc.).
threadprivate(var)	Used for global variables, gives each thread its own persistent copy across parallel regions.


When working on reduction we saw that, each thread gets a copy of variable in reduction and stores the result of the reduction inside the for loop, so if there are 5 threads and they all are counting to 10 then the reduction variable will store 10, and there will be a total of 5 reduction variables because of 5 threads. These 5 reduction variables will then be added back into the original variable one by one if the reduction type used was +