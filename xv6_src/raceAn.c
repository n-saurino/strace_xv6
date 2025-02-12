#include <stdlib.h>
#include <stdio.h>
// Here we are simply creating a memory block and performing a divide by 0 operation on it
//program fails for 2 cases -
// 1. if memory is not freed
// 2. if divide by 0 is performed
void simple_leak(){
   int n=5;
   int m=0;
   int *ptr = (int*)malloc(n * sizeof(int));
     for (int i = 0; i < n; ++i) {
            ptr[i] = i *100;
            ptr[i]/=m; //fail here as divide by 0
        }
    free(ptr);
}
int main(int argc, char *argv[]) {
   simple_leak();
    return (0);
}















