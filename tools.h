#ifndef TOOLSH
#define TOOLSH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>


void *_Malloc(size_t size,int *err);

template <typename T>
size_t _array_size(T *arr);

std::string _char_to_str(char *str);

char *_str_to_char(std::string str);

struct token{
  int start_index;
  int lost_index;
  char str[128];  
};

token *_str_to_tokens(const char *str,int *arrlen,int *cnt_tk);// разделяет строку по проблам на массив меньших строк - токенов
char **str_to_struct_tokens(const char *str,int *arrlen,int *cnt_tk);


#endif 