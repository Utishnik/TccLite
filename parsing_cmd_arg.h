#ifndef PARCING_CMD
#define PARCING_CMD

enum {PARCING_STR,PARSING_CHR,PARSING_DOUBL,PARSING_INTGER,PARCING_WARN,PARCING_ERR};

struct PARSING_STRING{
    char *str;
    int start_indx;
    int lost_indx;
};


struct PARSING_CHAR{
    char chr;
    int start_indx;
    int lost_indx;
};

struct PARSING_DOUBLE{
    char doubl;
    int start_indx;
    int lost_indx;
};

struct PARSING_WARNING{
    char *msg;
    int str_warn_fnd;
    int col_warn_fnd;
    char *str;
};

struct PARSING_ERROR{
    char *msg;
    int str_err_fnd;
    int col_err_fnd;
    char *str;
};

struct PARSING_INTEGER{
    char intger;
    int start_indx;
    int lost_indx;
};

#endif