
- `pt01.cc` create a thread that prints hello
- `pt02.cc` pass an arguement to the created thread
- `pt03.cc` create two threads that print stuff and see messed up

- `pt04.cc` same as 04 but use a mutex instead
	- Rule for a mutex is that the thread that unlocks it MUST have been the last one to have locked it!
	- This rule can allow a mutex to run faster than a binary semaphore.
	- This also allows for simpler initialization by assignment of 

- `pt05.cc` same as 04 but use a binary semaphore to fix printing problem.
	- The rules on which thread can unlock it is relaxed but not impactful in this example.
	- Only shown as a side-by-side comparison to the mutex.

- `pt06.cc` create a C++ object and pass to thread via pointer

