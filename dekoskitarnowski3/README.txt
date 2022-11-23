To compile gcc makingmusic.c -o makingmusic –lpthread

To run  ./makingmusic [-nodelay/-randomdelay] [# of vocalists] [# of composers] [# of soundproof rooms] [if -randomdelay maxWanderTime] [if -randomdelay maxSoundRoomUsageTime]



Some issues:
    Sometimes makes it all the way to the end of the program, others it gets held up with all the threads being stuck at "Ready to make music"
