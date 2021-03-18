# Concurrent UNIX Processes and shared memory
To compile run the command:
```
make
```
To clean run the command:
```
make clean
```
## Purpose
The goal of this homework is to become familiar with concurrent processing in Linux using shared memory. The task is fairly simple but the trick is to implement it with multiple processes using shared memory and signals to communicate between processes, and to exercise some control over them.

## Task
Write a program to compute the sum of n integers using a binary tree of processes. Assume that n is a power of 2. If you have fewer than 2k numbers, fill in other numbers with 0 to make it a power of 2. You will achieve the task as follows:
Partition the n integers into n/2 pairs. Use n/2 processes to add each pair of integers resulting into n/2 integers. Repeat the method on the n/2 integers and continue until the final result is obtained. (This is a binary tree algorithm.)
Structure the integers into a file, one integer per line. You will need to store the integers in shared memory and the child processes will store the results back in shared memory.
The main program will set up shared memory to read integers, set up a signal handler, and then, read all integers into shared memory from specified file. Since you do not know the number of integers to read, you may have to run two passes over the data file to find the number of integers and to actually read in the integers. Then, the main program will fork child processes who will create more children as needed. The children will test the index assigned to them, perform the computation, and write the result in place of the first integer. Further, the child process will also update a log file, named adder_log to say what has been done. You will have to use the code for multiple process ipc problem (Solution 4 in notes) to protect the critical resource – the log file. That is, the code to update the log file will constitute the critical section in each process.
Make sure you never have more than 20 processes in the system at any time. Add the pid of the child to the log file as you update it. The preferred output format is:
```
Time  PID   Index    Depth
```
where Index is the logical process id created by your master process and Depth is how deep in the tree your process is.
The first line shows numbers in consecutive indices of shared memory array with depth 3 (log8), the second line shows the sum of consecutive pairs with results in the first index (even index), the third line shows the results from pairs of second line with result in first index (divisible by 4), and so on.
The child process will be execed by the command
    

## Invoking the solution
master should take in several command line options as follows:
```
master -h
master [-h] [-s i] [-t time] datafile
-h        Describe how the project should be run and then, terminate.
-s x      Indicate the number of children allowed to exist in the system at the same time. (Default 20)
-t time   The time in seconds after which the process will terminate, even if it has not finished. (Default 100)
datafile   Input file containing one integer on each line.
```
Note that all of these options should have some sensible default values, which should be described if -h is specified. With the option -s i, master will fork/exec (i − 1) child processes but then not create any more until one of them terminates. Once a child terminates, master will create another, continuing this until it has solved the problem. The default for i is 20. At that point it will wait until all children have terminated. When done, it would output appropriate information to the logfile, called output.log. The option -t time allows you to specify maximum time for process execution (default 100 sec) and if it has not terminated by then, abort it.
You will be required to create separate bin_adder processes from your main process. That is, the main process will just spawn the child processes and wait for them to finish. The main process also sets a timer at the start of computation to specified time in seconds (default 100). If computation has not finished by this time, the main process kills all the spawned processes and then exits. Make sure that you print appropriate message(s).
In addition, the main process should print a message when an interrupt signal (^C) is received. All the children should be killed as a result. All other signals are ignored. Make sure that the processes handle multiple interrupts correctly. As a precaution, add this feature only after your program is well debugged.
The code for main and child processes should be compiled separately and the executables be called master and bin_adder. Make sure that you do not use absolute path for the child processes.
Termination Criteria: There are several termination criteria. First, if all the children have finished, the parent process should deallocate shared memory and terminate.
In addition, I expect your program to terminate after the specified amount of time (-t option) of real clock. This can be done using a timeout signal, at which point it should kill all currently running child processes and terminate. It should also catch the ctrl-c signal, free up shared memory and then terminate all children. No matter how it terminates, master should also output the value of the shared clock to the output file. For an example of a periodic timer interrupt, you can look at p318 in the text, in the program periodicasterik.c.


