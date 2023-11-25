#include <stdio.h>
#include <stdlib.h>
#include "baza_data_main_func.h"
#include "tools.h"
#include "parsing_cmd_arg.h"
#include "tcc_remake_var_check.h"
#include <stdarg.h>

typedef struct TCCOption {
    const char *name;
    unsigned short index;
    unsigned short flags;
} TCCOption;

enum { ERROR_WARN, ERROR_NOABORT, ERROR_ERROR };

#define WARN_STR_TYPE_IN_INT 0x01
#define WARN_FLOAT_TYPE_IN_INT 0x0f
#define WARN_NULL_TYPE_IN_INT 0x01

typedef struct CString {
    int size; /* size in bytes */
    int size_allocated;
    void *data; /* either 'char *' or 'nwchar_t *' */
    struct CString *prev;
} CString;

#define WARN_ON  1 /* warning is on (-Woption) */
#define WARN_ERR 2 /* warning is an error (-Werror=option) */
#define WARN_NOE 4 /* warning is not an error (-Wno-error=option) */

int Debug_Print(const char *fmt,...)
{
    va_list ap;
    int cnt_arg=counter_symbol_in_str(fmt,'%');
    va_start(ap,cnt_arg);
    int len=strlen(fmt);
    unsigned char *type_print =(unsigned char*)_Malloc(sizeof(unsigned char)*cnt_arg,0);
    int iter=0;
    for(int i=0;i<len;i++)
        if(fmt[i]=='%')
        {
            char s_next=fmt[i+1];
            if(s_next=='s')
                type_print[iter]=PARCING_STR;
            if(s_next=='d')
                type_print[iter]=PARSING_INTGER;
            if(s_next=='g')
                type_print[iter]=PARSING_DOUBL;
            if(s_next=='c')
                type_print[iter]=PARSING_CHR;
            if(s_next=='e')
                type_print[iter]=PARCING_ERR;
            if(s_next=='w')
                type_print[iter]=PARCING_WARN;
            iter++;
            /*
            добавить обработку маркера f
            он указует на строку fmt 
            */
        }

    for(int i=0;i<cnt_arg;i++)
    {
        switch (type_print[i])
        {
            case PARCING_STR:
                va_arg(ap,PARSING_STRING);
            break;

            case PARSING_INTGER:
                va_arg(ap,PARSING_INTEGER);
            break;

            case PARSING_DOUBL:
                va_arg(ap,PARSING_DOUBLE);
            break;

            case PARSING_CHR:
                va_arg(ap,PARSING_CHAR);
            break;

            case PARCING_ERR:
                va_arg(ap,PARSING_ERROR);
            break;

            case PARCING_WARN:
                va_arg(ap,PARSING_WARNING);
            break;

            default:
                printf("%c\t%s\n",type_print[i],"Error:\tunknown marker");
            break;
        }
    }
    va_end(ap);    
}

#define DEC_LEN 10

char DEC[DEC_LEN]={0,1,2,3,4,5,6,7,8,9};

bool is_string(char *str)
{
    bool res=true;
    int len=strlen(str);
    for(int i=0;i<len;i++)
    {
        bool num=false;
        bool point=false;
        for(int j=0;j<DEC_LEN;j++)
            if(str[i]==DEC[j]) {num=true;break;}
        if(!num) if(str[i]=='.') point=true;
        if(!num&&!point) return true;
    }
    return false;
}


int str_check_var(char *str_var)
{
    bool is_float=false;
    bool is_str=false;
    bool is_int=true;
    int bt=0;
    int len=strlen(str_var);
    is_str=is_string(str_var);

    if(!is_str)
    {
        int cnt=counter_symbol_in_str(str_var,'.');
        if(cnt==1) 
        {
            is_float=true;is_int=false;
            bt=VT_FLOAT;
        }
        if(cnt>1) 
        {
            is_float=false;is_int=false;is_str=true;
            bt=VT_STR;
        }
    }
    if(!bt) bt=VT_INT;
    return bt;
}

#define PACK_STACK_SIZE 8

struct Remake_TCCState {
    unsigned char verbose; /* if true, display some information during compilation */
    unsigned char nostdinc; /* if true, no standard headers are added */
    unsigned char nostdlib; /* if true, no standard libraries are added */
    unsigned char nocommon; /* if true, do not use common symbols for .bss data */
    unsigned char static_link; /* if true, static linking is performed */
    unsigned char rdynamic; /* if true, all symbols are exported */
    unsigned char symbolic; /* if true, resolve symbols in the current module first */
    unsigned char filetype; /* file type for compilation (NONE,C,ASM) */
    unsigned char optimize; /* only to #define __OPTIMIZE__ */
    unsigned char option_pthread; /* -pthread option */
    unsigned char enable_new_dtags; /* -Wl,--enable-new-dtags */
    unsigned int  cversion; /* supported C ISO version, 199901 (the default), 201112, ... */

    /* C language options */
    unsigned char char_is_unsigned;
    unsigned char leading_underscore;
    unsigned char ms_extensions; /* allow nested named struct w/o identifier behave like unnamed */
    unsigned char dollars_in_identifiers;  /* allows '$' char in identifiers */
    unsigned char ms_bitfields; /* if true, emulate MS algorithm for aligning bitfields */

    /* warning switches */
    unsigned char warn_none;
    unsigned char warn_all;
    unsigned char warn_error;
    unsigned char warn_write_strings;
    unsigned char warn_unsupported;
    unsigned char warn_implicit_function_declaration;
    unsigned char warn_discarded_qualifiers;
    #define WARN_ON  1 /* warning is on (-Woption) */
    unsigned char warn_num; /* temp var for tcc_warning_c() */

    unsigned char option_r; /* option -r */
    unsigned char do_bench; /* option -bench */
    unsigned char just_deps; /* option -M  */
    unsigned char gen_deps; /* option -MD  */
    unsigned char include_sys_deps; /* option -MD  */

    /* compile with debug symbol (and use them if error during execution) */
    unsigned char do_debug;
    unsigned char dwarf;
    unsigned char do_backtrace;
#ifdef CONFIG_TCC_BCHECK
    /* compile with built-in memory and bounds checker */
    unsigned char do_bounds_check;
#endif
    unsigned char test_coverage;  /* generate test coverage code */

    /* use GNU C extensions */
    unsigned char gnu_ext;
    /* use TinyCC extensions */
    unsigned char tcc_ext;

    unsigned char dflag; /* -dX value */
    unsigned char Pflag; /* -P switch (LINE_MACRO_OUTPUT_FORMAT) */

#ifdef TCC_TARGET_X86_64
    unsigned char nosse; /* For -mno-sse support. */
#endif
#ifdef TCC_TARGET_ARM
    unsigned char float_abi; /* float ABI of the generated code*/
#endif

    unsigned char has_text_addr;
    unsigned section_align; /* section alignment */
#ifdef TCC_TARGET_I386
    int seg_size; /* 32. Can be 16 with i386 assembler (.code16) */
#endif

    char *tcc_lib_path; /* CONFIG_TCCDIR or -B option */
    char *soname; /* as specified on the command line (-soname) */
    char *rpath; /* as specified on the command line (-Wl,-rpath=) */
    char *elf_entryname; /* "_start" unless set */
    char *init_symbol; /* symbols to call at load-time (not used currently) */
    char *fini_symbol; /* symbols to call at unload-time (not used currently) */
    char *mapfile; /* create a mapfile (not used currently) */

    /* output type, see TCC_OUTPUT_XXX */
    int output_type;
    /* output format, see TCC_OUTPUT_FORMAT_xxx */
    int output_format;
    /* nth test to run with -dt -run */
    int run_test;

    /* array of all loaded dlls (including those referenced by loaded dlls) */
    int nb_loaded_dlls;

    /* include paths */
    char **include_paths;
    int nb_include_paths;

    char **sysinclude_paths;
    int nb_sysinclude_paths;

    /* library paths */
    char **library_paths;
    int nb_library_paths;

    /* crt?.o object path */
    char **crt_paths;
    int nb_crt_paths;

    /* -D / -U options */
    /* -include options */

    /* error handling */
    void *error_opaque;
    void (*error_func)(void *opaque, const char *msg);
    int error_set_jmp_enabled;
    int nb_errors;

    /* output file for preprocessing (-E) */

    /* for -MD/-MF: collected dependencies for this compilation */
    char **target_deps;
    int nb_target_deps;

    /* compilation */

    int *ifdef_stack_ptr;

    /* included files enclosed with #ifndef MACRO */
    int nb_cached_includes;

    /* #pragma pack stack */
    int pack_stack[PACK_STACK_SIZE];
    int *pack_stack_ptr;
    char **pragma_libs;
    int nb_pragma_libs;

    /* inline functions are stored as token lists and compiled last
       only if referenced */
    int nb_inline_fns;

    /* sections */
    int nb_sections; /* number of sections, including first dummy section */

    int nb_priv_sections; /* number of private sections */

    /* predefined sections */
#ifdef CONFIG_TCC_BCHECK
    /* bound check related sections */
    Section *bounds_section; /* contains global data bound description */
    Section *lbounds_section; /* contains local data bound description */
#endif
    #define qrel s1->qrel

#ifdef TCC_TARGET_RISCV64
    struct pcrel_hi { addr_t addr, val; } last_hi;
    #define last_hi s1->last_hi
#endif

#ifdef TCC_TARGET_PE
    /* PE info */
    int pe_subsystem;
    unsigned pe_characteristics;
    unsigned pe_file_align;
    unsigned pe_stack_size;
    addr_t pe_imagebase;
# ifdef TCC_TARGET_X86_64
    Section *uw_pdata;
    int uw_sym;
    unsigned uw_offs;
# endif
#endif

#if defined TCC_TARGET_MACHO
    char *install_name;
    uint32_t compatibility_version;
    uint32_t current_version;
#endif

#ifndef ELF_OBJ_ONLY
    int nb_sym_versions;
    int nb_sym_to_version;
    int *sym_to_version;
    int dt_verneednum;
#endif

#ifdef TCC_IS_NATIVE
    const char *runtime_main;
    void **runtime_mem;
    int nb_runtime_mem;
#endif

#ifdef CONFIG_TCC_BACKTRACE
    int rt_num_callers;
#endif

    /* benchmark info */
    int total_idents;
    int total_lines;
    unsigned int total_bytes;
    unsigned int total_output[4];

    /* option -dnum (for general development purposes) */
    int g_debug;

    /* used by tcc_load_ldscript */
    int fd, cc;

    /* for warnings/errors for object files */
    const char *current_filename;

    /* used by main and tcc_parse_args only */
    int nb_files; /* number thereof */
    int nb_libraries; /* number of libs thereof */
    char *outfile; /* output filename */
    char *deps_outfile; /* option -MF */
    int argc;
    char **argv;
};

Remake_TCCState state_parsing;

void init_remk_tccstate(Remake_TCCState *r_tcc)
{
    r_tcc->warn_error=0;
    r_tcc->warn_num=0;
    r_tcc->warn_all=0;
}


enum {ERR,ERR_WARN};

#define WANR_NONE -1

static int error1(int mode,const Remake_TCCState err_state,const char *fmt,...)
{
    va_list ap;
    int cnt_arg=counter_symbol_in_str(fmt,'%');
    va_start(ap,cnt_arg);
    CString cs;
    if (mode == ERROR_WARN) {
            unsigned char wopt=err_state.warn_write_strings;
            unsigned char wopt1=err_state.warn_num;
            unsigned char wopt2=err_state.warn_error;
            if (0 == (wopt & WARN_ON))
                printf("warning\n");//присоединить va_list
            if (wopt & WARN_ERR)
                mode = ERROR_ERROR;
            if (wopt & WARN_NOE)
                mode = ERROR_WARN;
            
            if(wopt1&WARN_NULL_TYPE_IN_INT)
            {
                Debug_Print("%s %f","warning type 0 invalid value\t",fmt,ap);
                Debug_Print(fmt,ap);
            }
            if(wopt1&WARN_FLOAT_TYPE_IN_INT)
            {
                Debug_Print("%s %f","warning type the data type that should have been an integer is real\t",fmt,ap);
                Debug_Print(fmt,ap);
            }
            if(wopt2&WARN_STR_TYPE_IN_INT)
            {
                Debug_Print("%s %f","warning type the data type that should have been an integer is string\t",fmt,ap);
                Debug_Print(fmt,ap);
            }
        
        
        if (err_state.warn_none)
            return WANR_NONE;
    }

    if(!cnt_arg) 
    {
        Debug_Print("%s\tSTR_NUM = %d\tFILE = %s\tFUNCTION = %s","warning !cnt_arg",__LINE__,__FILE__,__func__);
        return (0x1fff);
    }

    if(mode==ERROR_ERROR) return ERR;
    if(mode==ERROR_WARN) return ERR_WARN;
    va_end(ap);
}

void _tcc_remake_warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    error1(ERROR_WARN,state_parsing,fmt,ap);
    va_end(ap);
    state_parsing.warn_write_strings-=0x01;
}

void _tcc_remake_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    error1(ERROR_ERROR, state_parsing,fmt,ap);
}

typedef void (*TCCErrorFunc)(void *opaque, const char *msg);

void tcc_set_error_func(Remake_TCCState *s, void *error_opaque, TCCErrorFunc error_func)
{
    s->error_opaque = error_opaque;
    s->error_func = error_func;
}

void Error_Func1(void *opaque,const char *msg)
{
    Debug_Print("%s\n",msg);
}

int tcc_parse_args(Remake_TCCState *s, int *pargc, char ***pargv, int optind)
{
    init_remk_tccstate(&state_parsing);
    Remake_TCCState *s1 = s;
    const TCCOption *popt;
    const char *optarg, *r;
    const char *run = NULL;
    int x;
    int tool = 1, arg_start = 0, noaction = optind;
    char **argv = *pargv;
    int argc = *pargc;

    while (optind < argc) 
    {
        r = argv[optind];
        optind++;
        if(tool) // alpha 0.1.2 open ровняет tool к 0 а close к 1 так же в alpha 0.2 если выполняются запросы к фаилу cfg и tool!=0 то выдастца ошибка
        {
            if (r[0] == 'B' && r[1] == 'D' && r[2] == ' ')
            {
                check1:
                if(!tool) tcc_set_error_func(&state_parsing,0,Error_Func1);
                state_parsing.error_func(0,"error tool == 0; close cnf file");
                r=argv[optind];
                if(!strlen(r))
                    state_parsing.warn_write_strings+=0x01;
                std::string path_f=r;

                ++s->verbose;
            }

            _tcc_remake_warning("%d = line\t%s = file\t%s\n",__LINE__,__FILE__,__func__);
            continue;
        }
        goto check1;

        if(str_tojdesto(r,"write"))
        {
            //write logic
            char *r_next=argv[optind];
            if(!strlen(r_next)) 
                state_parsing.warn_write_strings=0x01;
            if(!atoi(r_next)) 
                state_parsing.warn_num+=0x01;
            int get_bt=str_check_var(r_next);
            if(is_float(get_bt))
                state_parsing.warn_num+=0x0f;
            if(is_string(r_next))
                state_parsing.warn_error+=0x01;
            
            _tcc_remake_warning("%d = line\t%s = file\t%s\n",__LINE__,__FILE__,__func__);
        }
    }
}