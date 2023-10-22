#include "tools.h"
#include <string>
#include <string.h>

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
            printf("malloc warning; ptr = NULL\n");
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

std::string _char_to_str(char *str)
{
    #ifdef memory_debug
        if(str==NULL) { printf("warning str = NULL\n"); return "";}
    #endif
    std::string res=" ";
    int len=strlen(str);

    for(int i=0;i<len;i++)
        res[i]=str[i];
    return res;
}

char *_str_to_char(std::string str)
{
    int len=str.length();
    #ifdef memory_debug
        if(len==NULL) {printf("str.size() = 0\n");return NULL;}  
    #endif
    char *res=(char*)_Malloc((sizeof(char)*len),NULL);

    for(int i=0;i<len;i++)
    {
        res[i]=str[i];
    }
    return res;
}