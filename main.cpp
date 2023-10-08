#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

//#define Debug1

using namespace std;

const int count_col = 2;
const int max_len_str = 256;
string path_bd = "baza_data.txt";
const int MaxCountToken = 1024;

const int type_cnt = 3;
const int max_len_type_name = 32;
char map_type[type_cnt][max_len_str] = {
	"Int",
	"String",
	"Double"
};


void write(char str[count_col]);
string* read();
void rwrite();

void write(char str[count_col][max_len_str])
{
	ofstream file;
	file.open(path_bd, ofstream::app);

	if (!file.is_open()) { printf("error open file!\n"); return; }
	for (int i = 0; i < count_col; i++) file << str[i] << " ";
	file << endl;
	file.close();

}

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

int* find_value_in_bd(char* value, string* db,int count_col,int count_str,int maxlentk);

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
	int count_probel = counter_probels_string(str);
	printf("%d\n", count_probel);
	char** tokens = (char**)malloc(sizeof(char*) * (count_probel + 10));

	for (int i = 0; i < count_probel+10; i++)
		tokens[i] = (char*)malloc(sizeof(char)  * strlen(str));


	printf("len = %ld\n", strlen(str));
	printf("db1\n");

	int itrator = 0;
	int iter_char = 0;

	add_str_end_probel(str);

	for(int i=0;i<strlen(str);i++)
	{
		if (str[i] != ' ')
		{
			tokens[itrator][iter_char] = str[i];
		}
		iter_char++;
		if (str[i] == ' ')
		{
			tokens[itrator][iter_char-1] = '\0';
			itrator++;
			iter_char = 0;
		}
	}


	int findtklen=strlen(token);

	int *list_number_token=(int*)malloc(sizeof(int)*len_list_token_number);
	



	int iteratorn2=0;
	for (int i = 0; i < itrator; i++)
	{
		int lentk=strlen(tokens[i]);

		if(lentk==findtklen)	
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
			else {list_number_token[iteratorn2]=i;iteratorn2++;}
		}
		else
		{
			continue;
		}
	}

	*len_ret=iteratorn2;

	for (int i = 0; i < (count_probel + 10); i++)
		free((void*)tokens[i]);

	free(tokens);

	

	if(iteratorn2>0) return list_number_token;
	else return NULL;

}

void char_str_init(char *str,const char *str2,int len) { for(int i=0;i<len;i++) str[i]=str2[i];}

void init2darr(char ***arr,int h,int w){
	(*(arr))=(char**)malloc(sizeof(**(arr))*h);
	for(int i=0;i<h;i++)
		(*(arr))[i]=(char*)malloc(sizeof((***(arr)))*w);
}

void free2darr(char ***arr,int h)
{
	for(int i=0;i<h;i++)
		free((*arr)[i]);
	free((*arr));
}

template <typename T>
void print2darr(T a,int h,int w)
{
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
			printf("%c",a[i][j]);
		printf("\n");
	}
}

int* find_value_in_bd(char* value, string* db,int count_col,int count_str,int maxlentk)
{
	char ***str=(char***)malloc(sizeof(*str)*count_str);

	for(int i=0;i<count_str;i++)
		str[i]=(char**)malloc(sizeof(**str)*count_col);

	for(int j=0;j<count_str;j++)
		for(int i=0;i<maxlentk;i++)
			str[j][i]=(char*)malloc(sizeof(***str)*maxlentk);


	char **arr_bd_str;

	init2darr(&arr_bd_str,count_str,maxlentk);

	printf("!\n");
	for(int i=0;i<count_str;i++)
		for(int j=0;j<db[i].size();j++)
		{
			printf("%c\n",arr_bd_str[i][j]);
			arr_bd_str[i][j]=db[i][j];
		}

	print2darr(arr_bd_str,count_str,maxlentk);
	

	free2darr(&arr_bd_str,count_str);
	printf("db11\n");


	for(int j=0;j<count_str;j++)
		for(int i=0;i<count_col;i++){
			printf("i\n");free(str[j][i]);}

	printf("db1\n");
	for(int i=0;i<count_str;i++)
	free(str[i]);

	free(str);
	return 0;
}


int main()
{
	string testdb[3] = {
		"   niger 1  ",
		"ilya niger  dyra",
		"aaa 3.14 "
	};

	char value[10] = "niger";
	find_value_in_bd(value,testdb,3,3,128);

	 char o1[1024] = "121 ilia pizdaliz ebaniy hhhh   gfffg 121 121  pi 344334";
	 char o2[1024] = "  ";

	int retlen=0;
	string *bd=read(&retlen);

	if(bd==NULL) printf("empty bazadata!\n");

	for(int i=0;i<retlen;i++)
	{
		cout <<bd[i]<<endl;
	}
}
