# Instructions for the TA to run our implementation :

### Part 1. 
You can directly run the strace on Anubis and refer our document to evaluate on what all system calls we covered to explain this part.

### Part 2.
To follow along with our implementation to perform the strace operation. Do:

a. 
strace on
echo hi

Followed by:
b. 
strace off

And then,
c.
strace run echo Hello

d. 
You can look into our code for the Ring buffer implementation.
You can directly run the :

strace dump

e. 
You can look into our code for the tracing and spawning child processes implementation.


### Part 3. 
You can try:

strace on
echo hi

Also, 
strace -e write 
echo hello

Similarly we can do for any system call or their combination.

The line
 strace -s -e write echo hi 

works perfectly for our implementation.

 Also note, For all of our flags, we implemented them to have the command run in the same line as the strace <flag> call as a demonstration that it only runs once like the “run” implementation above.


 We thought that this would be less confusing for the user who may not expect the flags to terminate after one run when they try to turn it on in one command and then use the flag for subsequent commands that they want to trace.  Our implementation for the strace command with flags takes the form: strace <flag> <flag e if included> <system call if flag e set> <command> (i.e. strace -s -e write echo hi). 



 We used similar logic to capture all of the flags for each shell command. We looped through every argument in the shell command and checked if any of them matched the four commands that we were asked to implement. We used global kernel variables to store the flags and a system call to set them after they’d been recorded in a local variable. We do all of the command/option parsing and execution of strace natively in the shell.



### Part 4. 
You can simply run:
strace -o trace_log.txt 
cat trace_log.txt

To allow all the output to be captured in this file(create one if not already created) and display over the console.


### Part 5. 
You can look into our race.c(XV6) and raceAn.c(Anubis-Linux) file to look into the memory leak. 
