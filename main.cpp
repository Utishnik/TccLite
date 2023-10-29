#include "baza_data_main_func.h"

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



int *find_token( char* str,  char* token,int len_list_token_number,int *len_ret)
{
	add_str_end_probel(str);
	int cnt_probel=counter_probels_string(str);
	char **tokens=(char**)malloc(sizeof(*tokens)*(cnt_probel+1));
	for(int i=0;i<(cnt_probel+1);i++)
		tokens[i]=(char*)malloc(sizeof(**tokens)*len_list_token_number);

	int cnt_tk=0;
	int lenstr=strlen(str);
	int lentk=strlen(token);

	int chari=0;
	int itrator=0;
	int *strtindxtk=(int*)malloc(sizeof(int)*(cnt_probel+1));

	strtindxtk[0]=0;
	int *tokenlen=(int*)malloc(sizeof(int)*(cnt_probel+1));

	for(int i=0;i<strlen(str);i++)
	{
		if (str[i] != ' ')
		{
			tokens[itrator][chari] = str[i];

		}
		chari++;
		if (str[i] == ' ')
		{
			tokens[itrator][chari-1] = '\0';
			itrator++;
			strtindxtk[itrator]=i+1;
			chari = 0;
		}
	}

	int *tkfnd_indx=(int*)malloc(sizeof(*tkfnd_indx)*cnt_tk);
	int itr=0;

	for (int i = 0; i < itrator; i++)
	{
		int lentk1=strlen(tokens[i]);

		if(lentk1==lentk)	
		{
			bool boolean1=false;
			for(int j=0;j<lentk;j++)
			{
				if(tokens[i][j]==token[j])
				{
					boolean1=true;
				}
				else
				{
					boolean1=false;
					break;
				}
			}
			if(boolean1==false) {continue;}
			else {tkfnd_indx[itr]=strtindxtk[i];itr++;(*len_ret)++;}
		}
		else
		{
			continue;
		}
	}
	if(!len_ret) return NULL;
	return tkfnd_indx;
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

int** find_value_in_bd(char* value, string* db,int count_col,int max_len_str,int maxlentk,int cntfndtk,int *arrln,bool *is_empty)
{
	////
	char **arr_db=(char**)malloc(sizeof(char*)*count_col);
	for(int i=0;i<count_col;i++) arr_db[i]=(char*)malloc(sizeof(char)*max_len_str);

	for(int i=0;i<count_col;i++)
	{
		for(int j=0;j<db[i].size();j++)
			arr_db[i][j]=db[i][j];
		arr_db[i][(db[i].size())]='\0';
		//printf("%ld  %ld\n",db[i].size(),strlen(arr_db[i]));
	}

	int **result=(int**)malloc(sizeof(int*)*count_col);
	bool empty=false;

	for(int i=0;i<count_col;i++)
	{
		int ln_rt=0;
		result[i]=find_token(arr_db[i],value,cntfndtk,&ln_rt);
	
		arrln[i]=ln_rt;

		if(ln_rt>0) empty=true;
		if(ln_rt<0) err_ptr(((arrln)+i));
	}

	if(is_empty) *is_empty=empty;
	for(int i=0;i<count_col;i++) free(arr_db[i]);
	free(arr_db);

	return result;
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
	printf("zagluska!\n");
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

int num_token_by_indx(int index,char *str) 
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
_token_w **fndarr_processing(int **fndarr,int *index_unique_col,int *arrlen,int **ret_size_res_arr)
{
	//todo сделать проверку чтобы в index_unique_col небыло одинаковых индексов
	if(index_unique_col==NULL) {printf("\nDEBUG fndarr_processing index_unique_col = NULL\n");return NULL;}
	if(arrlen==NULL) {printf("\nDEBUG fndarr_processing arrlen = NULL\n");return NULL;}
	int **result;
	unsigned int size_arr_indx_unqe_col=(sizeof(index_unique_col)/sizeof(index_unique_col[0]));
	unsigned int size_arrln=(sizeof(arrlen)/sizeof(arrlen[0]));
	_token_w **tk_w_arr=(_token_w**)malloc(sizeof(*tk_w_arr)*size_arrln);
	int *cnt_tk_unqe_pos=(int*)malloc(sizeof(int)*size_arrln);
	for(int i=0;i<size_arrln;i++)
	{
		tk_w_arr[i]=(_token_w*)malloc(sizeof(*(tk_w_arr[i]))*arrlen[i]);
		for(int j=0;j<arrlen[i];j++)
		{
			tk_w_arr[i][j].index=fndarr[i][j];
			//tk_w_arr[i][j].number=j;
			for(int k=0;k<size_arr_indx_unqe_col;k++)
				if(tk_w_arr[i][j].number==index_unique_col[k]) { tk_w_arr[i][j]._unique_pos=true;cnt_tk_unqe_pos[i]++;break;}
		}
	}

	//иницилизация
	_token_w **res=(_token_w**)malloc(sizeof(*res)*size_arrln);
	for(int i=0;i<size_arrln;i++)
		res[i]=(_token_w*)malloc(sizeof(**res)*cnt_tk_unqe_pos[i]);

	int i_iterator=0;
	int j_iterator=0;

	int *size_res=(int*)malloc(sizeof(int)*size_arrln);
	memset(size_res,0,(sizeof(int)*size_arrln));
	for(int i=0;i<size_arrln;i++)
	{
		for(int j=0;j<arrlen[i];j++)
			if(tk_w_arr[i][j]._unique_pos) 
			{
				 _token_w_init(&(res[i_iterator][j_iterator]),tk_w_arr[i][j].index,tk_w_arr[i][j].number,tk_w_arr[i][j]._unique_pos); 
				 j_iterator++;
				 size_res[i]++;
			}
		i_iterator++;
	}
	(*ret_size_res_arr)=size_res;

	return res;
	//в цикле оставлять только токены где уникальная позиция true
}

//записует в базу данных строку равная ровно количеству столбцов в ней и если уникальные 
//значение найдутся то вернет их адреса
//index_unique_col массив индексов которые указают на данные которые должны быть уникальными чтобы
//строка записалась
//пример допустим база данных где первый столбец это имена пользователей(уникальные) и второе это их баланс
//тогда index_unique_col его длина будет равна 1 и он будет равен 0 тоесть первой строке
//true если не найдено уникальных токенов  false если найдено
bool write_full_str_in_bd(char str[CNT_COL][MX_LN_STR_BD],string *bd,int cnt_str_in_bd,int *index_unique_col,
int maxlenstr,int maxlentk,int cntfndtk,int count_col_bd,int **str_find_index,int *err) //cntfndtk - максимальное колво индесков токенов которое может найти
{
	//str_find_index - массив индексов строк с найдеными уникальными значениями
	int count_unique_cols=(sizeof(index_unique_col)/sizeof(index_unique_col[0]));
	if(count_unique_cols>count_col_bd) *err=-1;
	if(count_unique_cols==0) {return *err=-2;}
	char **value_arr=NULL;
	init2darr(&value_arr,count_unique_cols,maxlenstr);

	int itrator=0;
	for(int i=0;i<count_unique_cols;i++)
	{	
		int ind=index_unique_col[i];
		{
			for(int k=0;k<strlen(str[ind]);k++) value_arr[itrator][k]=str[ind][k];
			itrator++;
		}
	}

	int **arrlen=0;
	int ***fndarr;
	init2darr(&arrlen,count_unique_cols,cnt_str_in_bd);
	init3darr(&fndarr,count_unique_cols,cnt_str_in_bd,cntfndtk);

	bool is_empty=true;
	bool empty_fnd_value=true;
	for(int i=0;i<count_unique_cols;i++)
	{	
		if(is_empty) bool empty_fnd_value=true;//нашлось ли строка с уникальным значением
		fndarr[i]=find_value_in_bd(value_arr[i],bd,count_col_bd,maxlenstr,maxlentk,cntfndtk,arrlen[i],&empty_fnd_value);
		if(empty_fnd_value==false) is_empty=false;
	}

	if(!is_empty) {write(str,bd,true);return true;}



	return true;
	//а если нет нечего то пишется строка и потом сохраняется в фаил
	//что бы сделать изменение значения нужно проверить есть ли оно и заменить нужный стобец на найденой строке на желаемое значение
}

using namespace std;
int main(void)
{
	char str[100]="minch car hui pi";

	int arrlen[10];
	int cnt_tk;
	token *tok = _str_to_tokens(str,arrlen,&cnt_tk);

	num_token_by_indx(10,str);
}
