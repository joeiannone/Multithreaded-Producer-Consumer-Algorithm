# Multithreaded Producer-Consumer Algorithm
<b>A multithreaded producer-consumer algorithm implemented in C</b>
<br>
A small producer-consumer implememtation that reads from stdin; in this case an infinite number<br>
generator that is piped in, read into finite sized queues, and sorted in real time based on whether<br>
they are of the fibonacci, prime, or 'divisible by 3 or 5' number sets.

<b>Instructions:</b><br>
<b>1.)<b/> make<br>
<b>2.)<b/> ./input | ./pc<br>
<b>3.)<b/> ctrl+c to end (SIGINT is handled by closing stdin which communicates to consumer threads to <br>
clear queues and exit gracefully.)
