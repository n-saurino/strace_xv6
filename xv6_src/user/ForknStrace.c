#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
// OUR file
#include "strace.h"

void performfork() {
    //Simple program which forks a parent to a child. To invoke and check the trace functionality
    int my_fork;
    for(int i = 0; i < 3; i++){
        my_fork = fork();
        if (my_fork == -1) {
            printf(1, "Fork can not be completed!\n");
            return;
        } else if (my_fork == 0) {
            close(open("compile_flags.txt", 0)); // fails to open and close the file (return value = -1)
            exit();
        } else {
            wait();
        }
    }
}
int main() {
    printf(1, "Process is getting traced.\n");
    strace(TRACE_ON);
    performfork();
    strace(TRACE_OFF);
    printf(1, "Both the Processes & forks are now traced.\n");
    strace(TRACE_ON | TRACE_FORK);
    performfork();
    strace(TRACE_OFF);
    printf(1, "Process not being traced.\n");
    performfork();
    exit();
}