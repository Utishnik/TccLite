#include "tools.h"
#include <string>
#include <string.h>
#include "baza_data_main_func.h" //todo убрать функцию counter_probels_string, init2darr, init3darr,free2darr и другие в tools

#define Max_Token_Len 1024

#define Dynamic_Max_Token_Len

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

token *_str_to_tokens(const char *str,int *arrlen,int *cnt_tk)
{
    size_t len=strlen(str);
    char *str_clone=(char*)_Malloc(sizeof(char)*len,0);
    for(int i=0;i<len;i++) str_clone[i]=str[i];
    add_str_end_probel(str_clone);
    int cnt_probel=counter_probels_string(str_clone);
    int max_token_len=0;

    #ifdef Dynamic_Max_Token_Len
        max_token_len=len;
    #endif

    #ifndef Dynamic_Max_Token_Len
        max_token_len=Max_Token_Len;
    #endif
    token *tokens_arr=(token*)_Malloc(sizeof(token)*cnt_probel,NULL);
    int itrator=0;
    int chari=0;
    for(int i=0;i<cnt_probel;i++) tokens_arr[i].start_index=0;
	for(int i=0;i<len+1;i++)
	{
		if (str_clone[i] != ' ')
		{
            if(!tokens_arr[itrator].start_index)tokens_arr[itrator].start_index=i;
			tokens_arr[itrator].str[chari] = str_clone[i];
            chari++;
		}
		if (str_clone[i] == ' ')
		{
            tokens_arr[itrator].lost_index=i;
			tokens_arr[itrator].str[chari] = '\0';
            arrlen[itrator]=chari;
			itrator++;
			chari = 0;
		}
	}
    (*cnt_tk)=itrator;
    return tokens_arr;
}

