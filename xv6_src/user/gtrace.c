#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"


int main(int argc, char *argv[]) {
    printf(2,"Global tracing value: %d\n",gtrace());
     exit();
}