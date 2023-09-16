#include <iostream>
#include <string>
#include <fstream>

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
void read();
void rwrite();

void write(char str[count_col][max_len_str])
{
	ofstream file;
	file.open(path_bd,ofstream::app);

	if (!file.is_open()) { printf("error open file!\n"); return; }
	for (int i = 0; i < count_col; i++) file<< str[i]<<" ";
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

void find_value_in_bd(char* value,string *db,int col_count,int str_len)
{

	string *find_bd=(string*)malloc(sizeof(string)*col_count);
	int* len_list = (int*)malloc(sizeof(int) * col_count);

	if(find_bd!=0)
		memset(find_bd, 0, sizeof(string) * col_count);
	for (int i = 0; i < col_count; i++)
	{
		string str_BD;
	    /*	for (int j = 0; (db->c_str())[i][j] != '\0';j++)
		{
			str_BD[j] = (db[i]->c_str())[j];
		}*/
		int len_s_bd=0;

		for (int j = 0; db[i][j] != '\0'; j++) { str_BD[j] = (db[i][j]);len_s_bd++; }
		

		//printf("%s\n", str_BD);


		//printf("\t [ %d ]\n", len_s_bd);

		string str_BD_no_probel_in_start_penis_hui_blyat_aaaa="";

		int iterator = 0;

		for (int k = 0; k < len_s_bd; k++)
		{
			if (str_BD[k] == ' ')
				iterator++;
			else
				break;
		}

		//printf("%d = i\n", iterator);

		for (int k = 0; k < len_s_bd-iterator; k++) str_BD_no_probel_in_start_penis_hui_blyat_aaaa[k] = '.';



		for (int k = iterator; k < len_s_bd; k++)
			str_BD_no_probel_in_start_penis_hui_blyat_aaaa[k-iterator] = str_BD[k];
		str_BD_no_probel_in_start_penis_hui_blyat_aaaa[len_s_bd-iterator] = '\0';

	
		for (int o = 0; o < len_s_bd; o++)
			find_bd[i][o] = str_BD_no_probel_in_start_penis_hui_blyat_aaaa[o];
		len_list[i] = len_s_bd;

	/*	for (int o = 0; o < len_s_bd; o++)
			printf("%c", find_bd[i][o]);
		printf("\n");*/

	}


	for (int i = 0; i < col_count; i++)
	{
		int cnt_tok = 0;
		string copy_str_bd = find_bd[i];

		int iterator_Tokens = 0;
		string tokens[MaxCountToken];
		int token_char_iterator = 0;


		for (int j = 0; j < len_list[i]; j++)
		{
			if (copy_str_bd[j] == ' ')
			{		
				tokens[iterator_Tokens][token_char_iterator + 1] = '\0';
				iterator_Tokens++;
				//tokens[iterator_Tokens] = " ";
				token_char_iterator = 0;
				cnt_tok++;
			}
			else {
				tokens[iterator_Tokens][token_char_iterator] = copy_str_bd[j];
				token_char_iterator++;
			}

		}
		//for (int j = 0; j < cnt_tok+1; j++) {
		//	for (int o = 0; o < len_list[i]; o++)
		//	{
		//		printf("%c", tokens[j][o]);
		//	}
		//	printf("\n");
		//}

		int str_number = -1;
		bool isfind = true;


		//printf("%d\n", cnt_tok);
		for (int j = 0; j < cnt_tok + 1; j++)
		{
			for (int o = 0; o < len_list[i]; o++)
			{
				printf("%c", tokens[j][o]);
			}
			printf("\n");
			if (strlen(value) == strlen(tokens[j].c_str()))
			{
				for (int k = 0; k < strlen(value); k++)
				{
					if (value[k] != tokens[j][k]) { break; isfind = false; }
				}
			}
			else
			{
				isfind = false;
				break;
			}

			if (isfind) {
				str_number=i+1;
				//printf("%d\n", str_number);
			}
			isfind = false;
		}
			
	
	}



}

void read()
{
	int double_iterator;
	int string_iterator;
	int int_iterator;

	ifstream rfile;
	rfile.open(path_bd);
	if (!rfile.is_open()) { printf("error open read file!!!\n"); return; }

	string str="";
	int count_str=0;
	while (!rfile.eof())
	{
		count_str++;
	}

	string* db = (string*)malloc(sizeof(string) * count_str);

	{
		int i = 0;
		while (!rfile.eof())
		{
			str = "";
			getline(rfile, str);
			db[i] = str;
			i++;
		}
	}



	rfile.close();

}

int main()
{
	//char str1[64]="3.1415";
	//char str2[64] = "niger";
	//char arr[count_col][256];

	//for (int i = 0; i < 64; i++)
	//{
	//	arr[0][i] = str1[i];
	//}
	//for (int i = 0; i < 64; i++)
	//{
	//	arr[1][i] = str2[i];
	//}

	//write(arr);

	string testdb[3] = {
		"   niger 1  ",
		"ilya niger  dyra",
		"aaa 3.14 "
	};

	char value[10] = "niger";

	find_value_in_bd(value, testdb, 3, 0);

	system("pause");
}
