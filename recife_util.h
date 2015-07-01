#include <regex.h>


/* The following is the size of a buffer to contain any error messages
   encountered when the regular expression is compiled. */

#define MAX_ERROR_MSG 0x1000

/* Compile the regular expression described by "regex_text" into
   "r". */

static int compile_regex (regex_t * r, const char * regex_text);

static int match_regex (regex_t * r, const char * to_match);

char* get_regex_group(const char *text, const char *regex, int group);

void strlower(char *s);

void strupper(char *s);