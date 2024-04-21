#include <string>
#include <stdio.h>
#include <vector>
#include <regex>

enum class Arithmetic{
    INT,
    FLOAT,
    UNSIGNED
};

struct Json_String{
    std::string value;
    bool replase_sub_str(std::string str1,std::string str2)
    {
        value = std::regex_replace(value, std::regex(str1), str2); // replace 'def' -> 'klm'
    }
};

enum class Json_Types{
    JNULL,             ///< null value
    OBJ,           ///< object (unordered set of name/value pairs)
    ARRAY,            ///< array (ordered collection of values)
    STRING,           ///< string value
    BOOL,          ///< boolean value
    INT,   ///< number value (signed integer)
    UNSIGNED,  ///< number value (unsigned integer)
    FLOAT,     ///< number value (floating-point)
    BIN,           ///< binary array (ordered collection of bytes)
    DISC         ///< discarded by the parser callback function
};

struct position_t
{
    /// the total number of characters read
    size_t chars_read_total = 0;
    /// the number of characters read in the current line
    size_t chars_read_current_line = 0;
    /// the number of lines read
    size_t lines_read = 0;

    /// conversion to size_t to preserve SAX interface
    constexpr operator size_t() const
    {
        return chars_read_total;
    }
};

struct Arithmetic_Type{
    Arithmetic Type;
    Arithmetic get_type() {return this->Type;}
    void set_type(Arithmetic arith) {this->Type=arith;}
};

struct Basic_Type{
    Json_Types Type;
    Json_Types get_type() {return this->Type;}
    void set_type(Json_Types type) {this->Type=type;}
};

struct InputAdapterType{
    char *json;
    int cur;
    char get_character()
    {
        ++cur;
        return json[cur];
    }
};

struct M_OBJ{
    int Ivalue;
    float Fvalue;
    std::string Svalue;
    void *ptr;//object and array 
    size_t len_mem;
};

struct Type_Map{
    Basic_Type Type;
    M_OBJ *ptr;
    std::string key;
};

class JsonLexer{

    public:
    std::string JSON;
    std::size_t size;
    InputAdapterType ia;

    /// whether comments should be ignored (true) or signaled as errors (false)
    const bool ignore_comments = false;

    std::string error_message;

    /// the current character
    long long current = -1;
    std::string token_string;

    /// whether the next get() call should just return current
    bool next_unget = false;

    /// the start position of the current token
    position_t position;

    /// raw input token string (for error messages)
    std::string token_string;

    /// buffer for variable-length tokens (numbers, strings)
    std::string token_buffer {}; // array token clearing if scaning new type_key хранит значение спарсиное с json 

    /// a description of occurred lexer errors
    const char* error_message = "";

    // number values
    int value_integer = 0;
    unsigned int value_unsigned = 0;
    float value_float = 0;

    /// the decimal point
    const char decimal_point_char = '.';
    
    Type_Map *type_map;
};