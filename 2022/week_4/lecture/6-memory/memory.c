#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    /* identifiying memroy faults with valgrind cli tool, usage: $ valgrind ./memory
        will produce 2 errors:
            Invalid write of size 4 (memory.c:14) : this is touching unaccessable memory
            12 bytes in 1 blocks are definitely lost in loss record 1 of 1 (memory.c:11) : this is memory leak, can see also in leak section of the output, memory allocated but never freed
    */
    int *x = malloc(3 * sizeof(int));  // allocate memroy for size of 3 ints using sizeof(), alternative to defining array with int x[3];
    x[1] = 72;
    x[2] = 73;
    x[3] = 33;  // mistake here
}