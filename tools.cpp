#include "tools.h"
#include "ConsoleLogger.h"
#include <string>

void *_Malloc(size_t size,int *err)
{

    if(size==0)
    {
        #ifdef memory_debug
            printf("size = 0\n");if(err) (*err)=-1;
        #endif
    }
    void *ptr=malloc(size);
    if(ptr==NULL) 
    {
        #ifdef memory_debug
            printf("malloc error; ptr = NULL\n");
        #endif
        if(err) (*err)=-2;
    }
}

void _Debug_Func(std::string function_name,void (*debug_function)(CConsoleLogger *Debug_Console))
{
    CConsoleLogger debug_console;
    std::string debug_msg="debug function ";
    std::string title=debug_msg+function_name;
    debug_console.Create(title.c_str());

    debug_function(&debug_console);
}