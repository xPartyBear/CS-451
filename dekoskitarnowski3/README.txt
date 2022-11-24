To compile gcc makingmusic.c -o makingmusic –lpthread

To run  ./makingmusic [-nodelay/-randomdelay] [# of vocalists] [# of composers] [# of soundproof rooms] [if -randomdelay maxWanderTime] [if -randomdelay maxSoundRoomUsageTime]

The program uses semaphores and threads to have vocalists and composers make music together in soundproof
rooms. There are 2 options when running the program "-nodelay" and "-randomdelay", nodelay will have
no wander time or room usage time, this means that vocalists and composers will be instantly done wandering
and instantly be done using the room. randomdelay will have the vocalists and composers wander for a random
time between 0 and what the user inputs(as an arg), and they will also use the room for a random time
between 0 and what the user inputs(as an arg)

Some issues:
    Sometimes "-nodelay" makes it all the way to the end of the program, others it gets held up with all the threads being stuck at "Ready to make music"
    "-randomdelay" will have all vocalists and composers leave
    sometimes the composer number will be wrong in the soundproof room text
