#ifndef TOOLSH
#define TOOLSH
#include <stdio.h>
#include <stdlib.h>
#include <string>

void *_Malloc(size_t size,int *err=NULL);

void _Debug_Func(std::string function_name,void (*debug_function)(CConsoleLogger *Debug_Console));
#endif TOOLSH