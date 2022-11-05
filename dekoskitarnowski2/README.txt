To compile gcc rrScheduler.c -o rrScheduler
           gcc prime.c -o prime

To run  ./rrScheduler [Input file name] [time quantum]

The program intakes a file and time quantum number and runs the processes in the input file
in a round-robin scheduler. The processes are children running the prime function for the time quantum until the process
has no more burst time left.

Some issues:
    Sometimes the CPU will get stuck, and you will have to press any key to unstuck it.(soft lockup)
    When only one process is running, it will suspend and resume it until it is completed.
    The last process goes for an extra time quantum until it is labelled as completed.