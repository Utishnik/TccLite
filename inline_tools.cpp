#include "inline_tools.h"

inline bool is_empty_string(std::string str)
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
