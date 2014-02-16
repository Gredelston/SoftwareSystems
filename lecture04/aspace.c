/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>

int global;

int main ()
{
    int local1 = 5;
    int local2 = 5;
    int local3 = 5;
    int local4 = 5;
    int local5 = 5;
    
    void *p = malloc(128);

    printf ("Address of main is %p\n", main);
    printf ("Address of global is %p\n", &global);
    printf ("Address of local1 is %p\n", &local1);
    printf ("Address of local2 is %p\n", &local2);
    printf ("Address of local3 is %p\n", &local3);
    printf ("Address of local4 is %p\n", &local4);
    printf ("Address of local5 is %p\n", &local5);
    printf ("Address of p is %p\n", p);
    
    return 0;
}
