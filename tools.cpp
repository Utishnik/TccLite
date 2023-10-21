#include "tools.h"
#include <string>

void *_Malloc(size_t size,int *err)
{

    if(size==0)
    {
        #ifdef memory_debug
            printf("size = 0\n");if(err) (*err)=-1;
        #endif
        return 0;
    }
    void *ptr=malloc(size);
    if(ptr==NULL) 
    {
        #ifdef memory_debug
            printf("malloc error; ptr = NULL\n");
        #endif
        if(err) (*err)=-2;
    }
    return ptr;
}

template <typename T>
size_t _array_size(T *arr)
{
    size_t result=(sizeof(arr)/sizeof(arr[0]));
    return result;
}



