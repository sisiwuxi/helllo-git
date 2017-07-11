#define MAIN_ERR(x) printf x;
#define MAIN_DBG(x) //printf x;
#define MAIN_DBG2(x) //printf x;

#define TRUE 1
#define FALSE 0
#define RANDOM_STR_LEN 11
#define MAX_STR_LEN 200//max length of symbol
#define MAX_KEYWORDS_LEN 50
#define NUM_RANDOM_GROUP 62//10 + 26 + 26


#define DIR_PATH "D:/self/c/search_replace_code/mnfs/"
#define MID_REPLACE " --> "

typedef enum
{
    SCRAMBLE_STR_TYPE_NULL,
    SCRAMBLE_STR_TYPE_SUSPEND,
    SCRAMBLE_STR_TYPE_FUNCTION,
    SCRAMBLE_STR_TYPE_VARIABLE,
    SCRAMBLE_STR_TYPE_KEYWORDS,
    SCRAMBLE_STR_TYPE_DWELL,
    SCRAMBLE_STR_TYPE_MACRODEF,//6
    SCRAMBLE_STR_TYPE_DIGIT,
    SCRAMBLE_STR_TYPE_STRING,
    SCRAMBLE_STR_TYPE_COMMENT_MULTI,
    SCRAMBLE_STR_TYPE_COMMENT_SINGLE,
    SCRAMBLE_STR_TYPE_SPACE,
    SCRAMBLE_STR_TYPE_TAB,
    SCRAMBLE_STR_TYPE_RETURN,
    SCRAMBLE_STR_TYPE_MAX,
}SCRAMBLE_STR_TYPE;

struct NOPRO_ENTITY
{
    char Temp_Record_Status;
    char Left_Parenthesis;
    char Right_Parenthesis;
};


