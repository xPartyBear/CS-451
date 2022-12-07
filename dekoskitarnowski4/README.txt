To compile gcc demandpaging.c -o demandpaging

To run  ./demandpaging [address file]

This program takes in a file of logical addresses and converts them into physical addresses using
FIFO (First In First Out) and LRU (Least Recently Used) algorithms. The physical addresses for each algorithm
are stored into files labelled FIFO and LRU. The program also find page faults for each algorithm based on the logical
address file.
