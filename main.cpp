#include "baza_data_main_func.h"
#include "inline_tools.h"

#define memory_debug //если memory_debug есть то _Malloc выводит отладочную информацию
//#define malloc _Malloc //заменяет malloc на функцию amlloc с оберткой для отладки находящияся в фаиле tools.cpp реализация tools.h прототип

//#define Debug1

struct Array2d {
	int H;
	int W;
};

char map_type[type_cnt][MX_LN_STR_BD] = {
	"Int",
	"String",
	"Double"
};

void bool_print(bool x)
{
	if(x) printf("true");
	else printf("false");
}

string path_bd = "baza_data.txt";

template <typename T>
void print2darr(T **a,int h,int w)
{
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
			printf("%c",a[i][j]);
		printf("\n");
	}
}



string* read();
void rwrite();

struct DOUBLE {
	double a;
	int col;
	int str;
};

struct STRING {
	string a;
	int col;
	int str;
};

struct INTEGER {
	int a;
	int col;
	int str;
};

struct Map_Value_BD_Str {
	int start_indx;
	int end_indx;
};


#define Max_Db_Size 1024
string  *read(int *retsize)
{
	int double_iterator;
	int string_iterator;
	int int_iterator;
	ifstream rfile;
	rfile.open(path_bd);
	if (!rfile.is_open()) { printf("error open read file!!!\n"); return NULL; }

	string str = "";
	int count_str = 0;

	string* db = (string*)malloc(sizeof(string) * Max_Db_Size);
	{
		int i = 0;
		while (!rfile.eof())
		{
			str = "";
			getline(rfile, str);
			db[i] = str;
			i++;
			count_str++;
		}
	}
	*retsize=count_str;
	rfile.close();
	return db;
}

int counter_probels_string(const char* str)
{
	int counter = 0;
	for (int i = 0; i < strlen(str); i++)
		if (str[i] == ' ') counter++;
	return counter;
}


void add_str_end_probel( char *str)
{
	int len=strlen(str);
	str[len]=' ';
	str[len+1]='\0';
}
bool str_tojdesto(char *str1,char *str2);

void debug_print(const char *str);

//ищет токены в троке и возвращает их
//len_ret - кол-во найденых токенов
struct token *find_token(const char* str,  char* token,int len_list_token_number,int *len_ret)
//len_list_token_number - максимально количество токенов которое можно найти, также присваевает номера начльный и конечный индексы
{
	int *arrlen=(int*)_Malloc(sizeof(int)*len_list_token_number,NULL);
	int cnt_tk=0;
	struct token *Token = _str_to_tokens(str,arrlen,&cnt_tk);
	if(!cnt_tk) debug_print("error cnt_tk = 0");

	int cnt_probel=counter_probels_string(str);
	struct token *result=(struct token*)_Malloc(sizeof(struct token)*(cnt_probel+1),NULL);

	int cnt_find_token=0;
	for(int i=0;i<cnt_tk;i++)
	{
		if(str_tojdesto(token,Token[i].str))
		{
			result[cnt_find_token]=Token[i];
			cnt_find_token++;
		}
	}
	(*len_ret)=cnt_find_token;
	return result;
	//перебор токенов, возвращать токены
}

bool str_tojdesto(char *str1,char *str2)
{
	int len1=strlen(str1);
	int len2=strlen(str2);
	if(len1!=len2) return false;

	int len=len1=len2;

	bool result=true;
	for(int i=0;i<len;i++)
	{
		if(str1[i]==str2[i])
		{
			result=true;
			continue;
		}
		else
		{
			result=false;
			break;
		}
	}
	return result;
}

inline void err_ptr(void *ptr)
{
	printf("error pointer; adress pointer = \t %p",ptr);
}

//ищет токен в базе данных возвращет двухмерный массив токенов в arrln записует кол-во найденых токенов по всем строкам
//arrln не иницилизирутся!
//is_empty не иницилизируется!
token** find_value_in_bd(char* value, string* db,int count_str,int max_len_str,int maxlentk,int cntfndtk,int *arrln,bool *is_empty)
//todo убрать все не нужные аргументы
{
	//todo оптимизировать,сделать чтоб возврщались не токены а строки где они находятся,начальный и конечный индексы
	int *cnt_probel_arr=(int*)_Malloc(sizeof(int)*count_str,NULL);
	for(int i=0;i<count_str;i++)
		cnt_probel_arr[i]=(counter_probels_string(db[i].c_str())+1);
	token **res=(token**)_Malloc(sizeof(token*)*count_str,NULL);
	for(int i=0;i<count_str;i++)
	{
		res[i]=(token*)_Malloc(sizeof(token)*cnt_probel_arr[i],NULL);
		int len_rt=0;
		res[i]=find_token(db[i].c_str(),value,cnt_probel_arr[i],&len_rt);
		arrln[i]=len_rt;
		if(!len_rt) is_empty[i]=true;
		else is_empty[i]=false;
	}
	return res;
}


void char_str_init(char *str,const char *str2,int len) { for(int i=0;i<len;i++) str[i]=str2[i];}

template <typename T>
void init2darr(T ***arr,int h,int w){
	(*(arr))=(T**)malloc(sizeof(**(arr))*h);
	for(int i=0;i<h;i++)
		(*(arr))[i]=(T*)malloc(sizeof((***(arr)))*w);
}

template <typename T>
void init3darr(T ****arr,int h,int w,int d)
{
	(*(arr))=(T***)malloc(sizeof(**(arr))*h);
		for(int i=0;i<h;i++)
		(*(arr))[i]=(T**)malloc(sizeof((***(arr)))*w);
	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++)
			((*arr))[i][j]=(T*)malloc(sizeof((****(arr)))*d);
}

template <typename T>
void free3darr(T ****arr,int h,int w)
{
	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++)
			free(((*arr))[i][j]);
	for(int i=0;i<h;i++)
		free((*arr)[i]);

	free((*arr));			
}


void free2darr(char ***arr,int h)
{
	for(int i=0;i<h;i++)
		free((*arr)[i]);
	free((*arr));
}

void _token_w_init(_token_w *a,int indx,int num,bool unique_pos)
{
	a->index=indx;
	a->number=num;
	a->_unique_pos=unique_pos;
}

//записует строку в базу данных(в сам массив строк и в фаил)
void write(char str[CNT_COL][MX_LN_STR_BD],string *bd,bool write_in_file)
{
	
}

void debug_print(const char *str) //todo import to debug_tools.cpp
{
	printf("%s\n",str);
	//todo write to file debug_msg.txt
}

// получение номера токена по индесу
// token1 token2 tooken3
// 0123456789ABCDEF....
//при вводе любого индеска из 7,8,9,A,B,C  вы получите 2 тоесть номер token2 .Если введете 2 то получите один номер token1

int num_token_by_indx(int index,char *str)  // использовать в поиске токенов в строке
{
	int *arrlen=(int*)_Malloc(sizeof(int)*100,NULL);
	int cnt_tk=0;
	token *tokens=_str_to_tokens(str,arrlen,&cnt_tk);	
	if(!cnt_tk) debug_print("error cnt_tk=0");
	for(int i=0;i<cnt_tk;i++)
	{
		if( (index>=tokens[i].start_index) && (index<=tokens[i].lost_index) )
		{
			printf("%d\n",i);
			break;
		}
	}
	return 0;
}

//будет обрабатывать массив который возращет функция find_value_in_bd() 
//проверяет наличие уникальных токенов на уникальных местах
//возвращает токены которые на своих уникальных местах
//res_len_arr - иницилизруется
//res_len_arr - это 
_token_w **fndarr_processing(struct token **fndarr,int *index_unique_col,int *arrlen,int count_str_in_db,int **res_len_arr)
{
	int err=0;
	_token_w **result=(_token_w**)_Malloc(sizeof(_token_w*)*count_str_in_db,&err);
	if(err==-1) printf("%d\n",__LINE__);
	int len_index_unque_col_arr=(sizeof(index_unique_col)/sizeof(index_unique_col[0]));
	for(int i=0;i<count_str_in_db;i++)
	{
		result[i]=(_token_w*)_Malloc(sizeof(_token_w)*arrlen[i],&err);
		if(err==-1) printf("%d\n",__LINE__);
	}

	(*res_len_arr)=(int*)_Malloc(sizeof(int)*count_str_in_db,0);


	int **cnt_find_unique_tk=(int**)_Malloc(sizeof(int)*count_str_in_db,0);

	int itrator_i,itrator_j;
	itrator_i=itrator_j=0;
	for(int i=0;i<count_str_in_db;i++)
	{
		cnt_find_unique_tk[i]=(int*)_Malloc(sizeof(int)*arrlen[i],0);
		for(int j=0;j<arrlen[i];j++)
		{
			for(int k=0;k<len_index_unque_col_arr;k++)
			{
				if(fndarr[i][j].number==index_unique_col[k])
				{
					result[itrator_i][itrator_j]._unique_pos=true;
					result[itrator_i][itrator_j].number=fndarr[i][j].number;
					result[itrator_i][itrator_j].index=fndarr[i][j].start_index;
					result[itrator_i][itrator_j].lost_index=fndarr[i][j].lost_index;
					break;
				}
			}
			itrator_j++;
		}
		itrator_i++;
		(*res_len_arr)[i]=itrator_j;
		itrator_j=0;
	}
	return result;
}

struct Data_return_Write_Full_Str_In_BD
{
	bool **is_empty_arr;
	union arr2d_fnd_tk_or_cnt_unique_tk_in_str_bd
	{//x32 - sizeof 4 	x64 - sizeof 8
		int **arrlen_arr;
		int **rt_len_arr_arr;
	}U;
};

typedef struct Data_return_Write_Full_Str_In_BD  DRW_in_bd; //abbreviated name


//записует в базу данных строку равная ровно количеству столбцов в ней и если уникальные 
//значение найдутся то вернет их адреса
//index_unique_col массив индексов которые указают на данные которые должны быть уникальными чтобы
//строка записалась
//пример допустим база данных где первый столбец это имена пользователей(уникальные) и второе это их баланс
//тогда index_unique_col его длина будет равна 1 и он будет равен 0 тоесть первой строке
//true если не найдено уникальных токенов  false если найдено
//is_empty-return = 1 если не нашлось уникальных токенов на уникальных позициях
//return_ иницилизируется
//ret_drw_in_find хранит двумерный булевый массив проверок на пустоту, и union кол-во найденых уникальных токенов или кол-во найденых
//уникальных токенов на уникальных местах
bool write_full_str_in_bd(string *writing_data,BD bd,char **unique_value_arr,int *array_in_unique_indx,string bazadata_path,int len_array_in_unique_indx,
int len_str_in_unqe_vle_arr,int cntcol_in_unique_vle_arr,_token_w ****return_,DRW_in_bd *ret_drw_in_find,int *index_unique_col,int mxlenstr=128,int mxlentk=128,int cntfndtk=128)
{
	int cnt_str_writing_data=bd.count_str_in_bd;
	int max_str_len=find_mx_len_str_array(writing_data,cnt_str_writing_data);

	string bd_file_path="";
	if(bazadata_path.length()==0) bd_file_path=path_bd;
	bool is_empt = is_empty_string(bazadata_path);
	if(is_empt) bd_file_path=path_bd;
	if(len_array_in_unique_indx>cntcol_in_unique_vle_arr) {return 0;debug_print("error len_array_in_unique_indx  > cntcol_in_unique_vle_arr");}

	char **workig_value;
	init2darr(&workig_value,len_array_in_unique_indx,len_str_in_unqe_vle_arr);

	for(int i=0;i<len_array_in_unique_indx;i++){
		strcpy(workig_value[i],unique_value_arr[array_in_unique_indx[i]]);
	}

	(*return_)=(_token_w***)_Malloc(sizeof(_token_w**),0);

	bool **is_empty_arr;
	int **arrlen_arr=(int**)_Malloc(sizeof(int*)*len_array_in_unique_indx,0);
	int **rt_len_arr_arr=(int**)_Malloc(sizeof(int*)*len_array_in_unique_indx,0);

	bool is_empty_return_=true;
	init2darr(&is_empty_arr,len_array_in_unique_indx,bd.count_str_in_bd);
	for(int i=0;i<len_array_in_unique_indx;i++)
	{
		int *arrlen=(int*)_Malloc(sizeof(int)*cntfndtk,0);
		token **arr=find_value_in_bd(workig_value[i],bd.bd,bd.count_str_in_bd,mxlenstr,mxlentk,cntfndtk,arrlen,is_empty_arr[i]);
		int *rt_len_arr;
		_token_w **rt_value = fndarr_processing(arr,index_unique_col,arrlen,bd.count_str_in_bd,&rt_len_arr);
		
		//debug code
			for(int i1=0;i1<bd.count_str_in_bd;i1++)
			{
				printf("str number  %d\t",(i1+1));
				for(int j2=0;j2<rt_len_arr[i1];j2++)
				{
					printf("is u pos %d  index = %d\t",rt_value[i1][j2]._unique_pos,rt_value[i1][j2].index);
				}
				printf("\n");
			}
		//

		for(int i1=0;i1<bd.count_str_in_bd;i1++)
			if(rt_value[i1]) {is_empty_return_=false;break;}

		rt_len_arr_arr[i]=rt_len_arr;
		arrlen_arr[i]=arrlen;
		(*return_)[i]=rt_value;
	}
	ret_drw_in_find->is_empty_arr=is_empty_arr;
	ret_drw_in_find->U.rt_len_arr_arr;
	
	if(is_empty_return_) 
	{

		return true;
	}
	else return false;
}

using namespace std;
int main(int argc,char *argv[])
{
	BD test_bd;
	string db_txt[3]={
		"minch 130",
		"car 190",
		"kurimo 11 1 kurimo"
	};
	test_bd.bd=db_txt;
	test_bd.count_str_in_bd=3;
	test_bd.len_str=100;

	char *value[3]={
		"2",
		"minch",
		"11111"
	};

	int ind_a[2]={0,2};
	int test_inx_u_col[2]={0,3};
	/*
	int arrln[3];
	bool is_emp[3];
	int ind_a[2]={1,2};
	printf("dd\n");
	token **arr = find_value_in_bd(value,db,3,100,100,100,arrln,is_emp);
	int *rt_len_arr;
	printf("dd\n");
	_token_w **t  = fndarr_processing(arr,ind_a,arrln,3,&rt_len_arr);

	if(rt_len_arr==0) {
		printf("err -1\n");
		return -11;
	}

	printf("dd\n");
	for(int i=0;i<3;i++)
	{
		printf("%d\n",rt_len_arr[i]);
		for(int j=0;j<rt_len_arr[i];j++)
		{
			printf("un pos=%d ",t[i][j]._unique_pos);
			printf("\tindex %d ",t[i][j].index);
		}
		printf("\n");
	}*/
	
	_token_w ***arr;
	DRW_in_bd test_drw;
	if(write_full_str_in_bd(test_bd,value,ind_a,"",2,128,3,&arr,&test_drw,test_inx_u_col))
		printf("true\n");
	else
		printf("false\n");

	
}
