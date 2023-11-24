#ifndef DEBUG
#define DEBUG
    #include <stdio.h>
    #include <stdlib.h>
    int counter=0;
    #define Debug_And_Counter() printf("cnt = %d\tstring=%d\tfile=%s\n",counter,__LINE__,__FILE__); counter++;
#endif