#ifndef BD_MAIN_FUNC
#define BD_MAIN_FUNC

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include "tools.h"
#include <chrono>

using namespace std;

#include "baza_data_main_func.h"

#define memory_debug //если memory_debug есть то _Malloc выводит отладочную информацию
//#define malloc _Malloc //заменяет malloc на функцию amlloc с оберткой для отладки находящияся в фаиле tools.cpp реализация tools.h прототип

#define memory_debug

//#define Debug1

const int CNT_COL = 2;
const int MX_LN_STR_BD = 256;
const int MaxCountToken = 1024;

const int type_cnt = 3;
const int max_len_type_name = 32;


template <typename T>
void print2darr(T **a,int h,int w);

string* read();


#define Max_Db_Size 1024
string  *read(int *retsize);

int counter_probels_string(const char* str); 

void add_str_end_probel( char *str); // добавляет в конец строки пробел(это нужно для работа find_token() )

token *find_token( char* str,  char* token,int len_list_token_number,int *len_ret);

bool str_tojdesto(char *str1,char *str2);

inline void err_ptr(void *ptr);

token** find_value_in_bd(char* value, string* db,int count_str,int max_len_str,int maxlentk,int cntfndtk,int *arrln,bool *is_empty);

void char_str_init(char *str,const char *str2,int len);

template <typename T>
void init2darr(T ***arr,int h,int w); 
//todo передовалось по значению
template <typename T>
void init3darr(T ****arr,int h,int w,int d);

template <typename T>
void free3darr(T ****arr,int h,int w);

void free2darr(char ***arr,int h);

struct Token_W{
	int index;
	int number;
	bool _unique_pos;
};
typedef struct Token_W _token_w;

void _token_w_init(_token_w *a,int indx,int num,bool unique_pos);

void write(char str[CNT_COL][MX_LN_STR_BD],string *bd,bool write_in_file);

_token_w **fndarr_processing(int **fndarr,int *index_unique_col,int *arrlen,int **ret_size_res_arr);

bool write_full_str_in_bd(char str[CNT_COL][MX_LN_STR_BD],string *bd,int cnt_str_in_bd,int *index_unique_col,
int maxlenstr,int maxlentk,int cntfndtk,int count_col_bd,int **str_find_index,int *err);

int num_token_by_indx(int index,char *str);

#endif
