#include <string>
#include <stdio.h>

using namespace std;

inline bool is_empty_string(string str)

    {   
	    int len=str.size();
	    bool res=false;
            for(int i=0;i<len;i++)
            {
                if(str[i]!=' ' && str[i]!='\t')                                                         {
                        res=true;
                        break;
                }
            }
	printf("%d\n",res);
	return res;
	    
    }

int main()
{
	string str1="  ";
	string str2="   2";
	string str3="111 ";

	is_empty_string(str1);
	is_empty_string(str2);
	is_empty_string(str3);			
}
