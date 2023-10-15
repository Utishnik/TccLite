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
void print2darr(T a,int h,int w)
{
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
			printf("%c",a[i][j]);
		printf("\n");
	}
}

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

	//print2darr(tokens,cnt_probel+1,len_list_token_number);

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

int** find_value_in_bd(char* value, string* db,int count_col,int count_str,int maxlentk,int cntfndtk,int **arrln)
{
	////
	char **arr_db=(char**)malloc(sizeof(char*)*count_col);
	for(int i=0;i<count_col;i++) arr_db[i]=(char*)malloc(sizeof(char)*count_str);

	for(int i=0;i<count_col;i++)
	{
		for(int j=0;j<db[i].size();j++)
			arr_db[i][j]=db[i][j];
		arr_db[i][(db[i].size())]='\0';
		printf("%ld  %ld\n",db[i].size(),strlen(arr_db[i]));
	}

	int **result=(int**)malloc(sizeof(int*)*count_col);

	for(int i=0;i<count_col;i++)
	{
		int ln_rt=0;
		result[i]=find_token(arr_db[i],value,cntfndtk,&ln_rt);
		//printf("%s   %d\n",arr_db[i],ln_rt);
	
		(*arrln)[i]=ln_rt;
	}

	//print2darr(result,count_col,cntfndtk);


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

void free2darr(char ***arr,int h)
{
	for(int i=0;i<h;i++)
		free((*arr)[i]);
	free((*arr));
}



template <typename T>
void *Malloc(T ptr,int size)
{
	return malloc(size*(*ptr));
}




int main()
{
	string testdb[3] = {
		"  gggniger  niger 1  ",
		"ilia  gggniger gggniger gggniger gggniger niger  dyra",
		"aaa 3.14 \0"
	};

	char value[10] = "gggniger";

	int *lenarray=(int*)malloc(sizeof(int)*3);
	int **arrret=find_value_in_bd(value,testdb,3,200,200,10,&lenarray);


   //int **arr=find_value_in_bd(value,testdb,3,3,123,128,&arr2d);






	//print2darr(arr,3,100);


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
