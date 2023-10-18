#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

//#define Debug1

struct Array2d {
	int H;
	int W;
};

using namespace std;


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

const int CNT_COL = 2;
const int MX_LN_STR_BD = 256;
string path_bd = "baza_data.txt";
const int MaxCountToken = 1024;

const int type_cnt = 3;
const int max_len_type_name = 32;
char map_type[type_cnt][MX_LN_STR_BD] = {
	"Int",
	"String",
	"Double"
};




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

void err_ptr(void *ptr)
{
	printf("error pointer; adress pointer = \t %p",ptr);
}

int** find_value_in_bd(char* value, string* db,int count_col,int max_len_str,int maxlentk,int cntfndtk,int **arrln,bool *is_empty)
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
	
		(*arrln)[i]=ln_rt;

		if(ln_rt>0) empty=true;
		if(ln_rt<0) err_ptr(((*arrln)+i));
	}

	*is_empty=empty;
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

//записует в базу данных строку равная ровно количеству столбцов в ней
//index_unique_col массив индексов которые указают на данные которые должны быть уникальными чтобы
//строка записалась
//пример допустим база данных где первый столбец это имена пользователей(уникальные) и второе это их баланс
//тогда index_unique_col его длина будет равна 1 и он будет равен 0 тоесть первой строке
bool write_full_str_in_bd(char str[CNT_COL][MX_LN_STR_BD],string *bd,int cnt_str_in_bd,int *index_unique_col,
int maxlenstr,int maxlentk,int cntfndtk,int count_col_bd,int **str_find_index) //cntfndtk - максимальное колво индесков токенов которое может найти
{
	//str_find_index - массив индексов строк с найдеными уникальными значениями
	int count_unique_cols=(sizeof(index_unique_col)/sizeof(index_unique_col[0]));
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

	int ***arrlen=0;
	int ***fndarr;
	init3darr(&arrlen,count_unique_cols,cnt_str_in_bd,count_col_bd);
	init3darr(&fndarr,count_unique_cols,cnt_str_in_bd,cntfndtk);

	bool is_empty=true;
	bool empty_fnd_value=true;
	for(int i=0;i<count_unique_cols;i++)
	{	
		if(is_empty) bool empty_fnd_value=true;//нашлось ли строка с уникальным значением
		fndarr[i]=find_value_in_bd(value_arr[i],bd,count_col_bd,maxlenstr,maxlentk,cntfndtk,arrlen[i],&empty_fnd_value);
		if(empty_fnd_value==false) is_empty=false;
	}

	// сделано получения уникальный столбцов осталось прогнать их
	// и если где то находится значение то возвращается ноль
	//а если нет нечего то пишется строка и потом сохраняется в фаил
	//что бы сделать изменение значения нужно проверить есть ли оно и заменить нужный стобец на найденой строке на желаемое значение
}



int main(int argc,char *argv[])
{
	string testdb[3] = {
		"  gggniger  niger 1  ",
		"ilia  gggniger gggniger gggniger gggniger niger  dyra",
		"aaa 3.14 \0"
	};

	char value[10] = "gggniger";


	int *lenarray=(int*)malloc(sizeof(int)*3);
	int **arrret=find_value_in_bd(value,testdb,3,200,200,10,&lenarray);

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<lenarray[i];j++)
		{
			//printf("%d ",arrret[i][j]);
		}
		//printf("\n");
	}

	 char o1[1024] = "121 ilia pizdaliz ebaniy hhhh   gfffg 121 121  pi 344334";
	 char o2[1024] = "  ";

	int retlen=0;
	string *bd=read(&retlen);

	if(bd==NULL) printf("empty bazadata!\n");

	#ifdef Debug
	for(int i=0;i<retlen;i++)
		cout <<bd[i]<<endl;
	#endif
}
