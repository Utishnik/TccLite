#include "inline_tools.h"

 bool is_empty_string(std::string str)
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
	    return res;
 }


inline void Bd_in_str_arr_init(std::string *str1,std::string *str2,int cnt_str)
{
    for(int i=0;i<cnt_str;i++)
    {
        //str1[i]
    }
}