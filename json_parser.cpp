#include <stdio.h>
#include <stdlib.h>
#include "json_lexer.cpp"
#include <assert.h>

char **JSON;
int current;
typedef unsigned long long uint64_t;
typedef char error_type;

//scan_type - scaning type if is Arithmetic 
//arith - set type
error_type get_arithmetic_value(const Basic_Type *scan_type,Arithmetic_Type *arith_type)
{
    switch (scan_type->Type)
    {
        case Json_Types::UNSIGNED:
        {
            arith_type->Type=Arithmetic::UNSIGNED;
            break;
        }
        case Json_Types::INT:
        {
            arith_type->Type=Arithmetic::INT;
            break;
        }
        case Json_Types::FLOAT:
        {
            arith_type->Type=Arithmetic::FLOAT;
            break;
        }

        case Json_Types::JNULL:
        case Json_Types::OBJ:
        case Json_Types::ARRAY:
        case Json_Types::STRING:
        case Json_Types::BOOL:
        case Json_Types::BIN:
        case Json_Types::DISC:
        default:
            return -1;            
    }
}

    const std::string& key(JsonLexer *JL) 
    {
       // assert(Type_Map.ptr != nullptr);
        Basic_Type jt=JL->type_map->Type;

        switch (jt.Type)
        {
            // use integer array index as key
            case Json_Types::ARRAY:
            {
                return JL->type_map->key;
            }

            // use key from the object
            case Json_Types::OBJ:
                return JL->type_map->key;

            // use an empty key for all primitive types
            case Json_Types::JNULL:
            case Json_Types::STRING:
            case Json_Types::BOOL:
            case Json_Types::INT:
            case Json_Types::UNSIGNED:
            case Json_Types::FLOAT:
            case Json_Types::BIN:
            case Json_Types::DISC:
            default:
                return "";
        }
    }

void int_to_string( std::string target, std::size_t value )
{
    // For ADL
    using std::to_string;
    target = to_string(value);
}

struct uchar_traits
{
    using char_type = unsigned char;
    using int_type = uint64_t;

    // Redefine to_int_type function
    static int_type to_int_type(char_type c) noexcept
    {
        return static_cast<int_type>(c);
    }

    static char_type to_char_type(int_type i) noexcept
    {
        return static_cast<char_type>(i);
    }

    static constexpr int_type eof() noexcept
    {
        return static_cast<int_type>(EOF);
    }
};

// Explicitly define char traits for signed char since it is not standard
struct schar_traits
{
    using char_type = signed char;
    using int_type = uint64_t;

    // Redefine to_int_type function
    static int_type to_int_type(char_type c) noexcept
    {
        return static_cast<int_type>(c);
    }

    static char_type to_char_type(int_type i) noexcept
    {
        return static_cast<char_type>(i);
    }

    static constexpr int_type eof() noexcept
    {
        return static_cast<int_type>(EOF);
    }
};


char get(JsonLexer *JL)
{
    JL->position.chars_read_total++; //++position.chars_read_total;
    JL->position.chars_read_current_line++;//++position.chars_read_current_line;

        if (JL->next_unget)
        {
                // just reset the next_unget variable and work with current
                JL->next_unget = false;
        }
        else
        {
            current = JL->ia.get_character();
        }

        if (current != JL->size)
        {
            JL->token_string.push_back(JL->JSON[current]);// строка которая очищается при новом токене тоетсь это не массив строк
        }

        if (current == '\n')
        {
            JL->position.lines_read++;
            JL->position.chars_read_current_line = 0;
        }

        return current;
}

 void unget(JsonLexer *JL)
{
        JL->next_unget = true;

        JL->position.chars_read_total--;

        // in case we "unget" a newline, we have to also decrement the lines_read
        if (JL->position.chars_read_current_line == 0)
        {
            if (JL->position.lines_read > 0)
            {
                JL->position.lines_read--;
            }
        }
        else
        {
            JL->position.chars_read_current_line--;
        }

        if (current != JL->size)
        {
            assert(JL->token_string.empty());
            JL->token_string.pop_back();
        }
}


while (is_ws_char(c))
{
	if ((!ADVANCE_CHAR(str, tok)) || (!PEEK_CHAR(c, tok)))
	goto out;
}
if (c == '/' && !(tok->flags & JSON_TOKENER_STRICT))
{
    printbuf_reset(tok->pb);
	printbuf_memappend_checked(tok->pb, &c, 1);
	state = json_tokener_state_comment_start;
}
else
{
    state = saved_state;
	goto redo_char;
}
			break;

		case json_tokener_state_start:
			switch (c)
			{
			case '{':
				state = json_tokener_state_eatws;
				saved_state = json_tokener_state_object_field_start;
				current = json_object_new_object();
				if (current == NULL)
				{
					tok->err = json_tokener_error_memory;
					goto out;
				}
				break;
			case '[':
				state = json_tokener_state_eatws;
				saved_state = json_tokener_state_array;
				current = json_object_new_array();
				if (current == NULL)
				{
					tok->err = json_tokener_error_memory;
					goto out;
				}
				break;
			case 'I':
			case 'i':
				state = json_tokener_state_inf;
				printbuf_reset(tok->pb);
				tok->st_pos = 0;
				goto redo_char;
			case 'N':
			case 'n':
				state = json_tokener_state_null; // or NaN
				printbuf_reset(tok->pb);
				tok->st_pos = 0;
				goto redo_char;
			case '\'':
				if (tok->flags & JSON_TOKENER_STRICT)
				{
					/* in STRICT mode only double-quote are allowed */
					tok->err = json_tokener_error_parse_unexpected;
					goto out;
				}
				/* FALLTHRU */
			case '"':
				state = json_tokener_state_string;
				printbuf_reset(tok->pb);
				tok->quote_char = c;
				break;
			case 'T':
			case 't':
			case 'F':
			case 'f':
				state = json_tokener_state_boolean;
				printbuf_reset(tok->pb);
				tok->st_pos = 0;
				goto redo_char;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
				state = json_tokener_state_number;
				printbuf_reset(tok->pb);
				tok->is_double = 0;
				goto redo_char;
			default: tok->err = json_tokener_error_parse_unexpected; goto out;
			}
			break;

bool is_eof(JsonLexer *JL)
{
    return !(JL->current!=JL->size);
}

bool scan_comment(JsonLexer *JL)
    {
        switch (get(JL))
        {
            // single-line comments skip input until a newline or EOF is read
            case '/':
            {
                while (true)
                {
                    switch (get(JL))
                    {
                        case '\n':
                        case '\r':
                        case '\0':
                            return true;

                        default:
                            if(is_eof(JL)) return true;
                            break;
                    }
                }
            }

            // multi-line comments skip input until */ is read
            case '*':
            {
                while (true)
                {
                    switch (get(JL))
                    {
                        case '\0':
                        {
                            JL->error_message = "invalid comment; missing closing '*/'";
                            return false;
                        }

                        case '*':
                        {
                            switch (get(JL))
                            {
                                case '/':
                                    return true;

                                default:
                                {
                                    unget(JL);
                                    continue;
                                }
                            }
                        }

                        default:
                            if(is_eof(JL)) {
                                JL->error_message = "invalid comment; missing closing '*/'";
                                return false;
                            }
                            continue;
                    }
                }
            }

            // unexpected character after reading '/'
            default:
            {
                JL->error_message = "invalid comment; expecting '/' or '*' after '/'";
                return false;
            }
        }
    }

template<typename BasicJsonType>
class lexer_base
{
  public:
    /// token types for the parser
    enum class token_type
    {
        uninitialized,    ///< indicating the scanner is uninitialized
        literal_true,     ///< the `true` literal
        literal_false,    ///< the `false` literal
        literal_null,     ///< the `null` literal
        value_string,     ///< a string -- use get_string() for actual value
        value_unsigned,   ///< an unsigned integer -- use get_number_unsigned() for actual value
        value_integer,    ///< a signed integer -- use get_number_integer() for actual value
        value_float,      ///< an floating point number -- use get_number_float() for actual value
        begin_array,      ///< the character for array begin `[`
        begin_object,     ///< the character for object begin `{`
        end_array,        ///< the character for array end `]`
        end_object,       ///< the character for object end `}`
        name_separator,   ///< the name separator `:`
        value_separator,  ///< the value separator `,`
        parse_error,      ///< indicating a parse error
        end_of_input,     ///< indicating the end of the input buffer
        literal_or_value  ///< a literal or the begin of a value (only for diagnostics)
    };

    /// return name of values of type token_type (only used for errors)
    static const char* token_type_name(const token_type t) noexcept
    {
        switch (t)
        {
            case token_type::uninitialized:
                return "<uninitialized>";
            case token_type::literal_true:
                return "true literal";
            case token_type::literal_false:
                return "false literal";
            case token_type::literal_null:
                return "null literal";
            case token_type::value_string:
                return "string literal";
            case token_type::value_unsigned:
            case token_type::value_integer:
            case token_type::value_float:
                return "number literal";
            case token_type::begin_array:
                return "'['";
            case token_type::begin_object:
                return "'{'";
            case token_type::end_array:
                return "']'";
            case token_type::end_object:
                return "'}'";
            case token_type::name_separator:
                return "':'";
            case token_type::value_separator:
                return "','";
            case token_type::parse_error:
                return "<parse error>";
            case token_type::end_of_input:
                return "end of input";
            case token_type::literal_or_value:
                return "'[', '{', or a literal";
            // LCOV_EXCL_START
            default: // catch non-enum values
                return "unknown token";
                // LCOV_EXCL_STOP
        }
    }
};