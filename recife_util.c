#include "recife_util.h"
//remember to remove after debug
//#include <stdio.h>
#include <pcre.h> 
#include <string.h>
#include <ctype.h>


static int compile_regex (regex_t * r, const char * regex_text)
{
    int status = regcomp (r, regex_text, REG_EXTENDED|REG_ICASE);
    if (status != 0) {
	char error_message[MAX_ERROR_MSG];
	regerror (status, r, error_message, MAX_ERROR_MSG);
        //printf ("Regex error compiling '%s': %s\n",
        //         regex_text, error_message);
        return 1;
    }
    return 0;
}

/*
  Match the string in "to_match" against the compiled regular
  expression in "r".
 */

static int match_regex (regex_t * r, const char * to_match)
{
    /* "P" is a pointer into the string which points to the end of the
       previous match. */
    const char * p = to_match;
    /* "N_matches" is the maximum number of matches allowed. */
    const int n_matches = 100;
    /* "M" contains the matches found. */
    regmatch_t m[n_matches];

    while (1) {
        int i = 0;
        int nomatch = regexec (r, p, n_matches, m, 0);
        if (nomatch) {
            //printf ("No more matches.\n");
            return nomatch;
        }
        for (i = 0; i < n_matches; i++) {
            int start;
            int finish;
            if (m[i].rm_so == -1) {
                break;
            }
            start = m[i].rm_so + (p - to_match);
            finish = m[i].rm_eo + (p - to_match);
            if (i == 0) {
                //printf ("$& is ");
            }
            else {
                //printf ("$%d is ", i);
            }
            //printf ("'%.*s' (bytes %d:%d)\n", (finish - start),
            //       to_match + start, start, finish);
        }
        p += m[0].rm_eo;
    }
    return 0;
}

char* get_regex_group(const char *text, const char *regex, int group) {
    //regex_t r;

    //compile_regex(& r, regex);
    //match_regex(& r, text);
    const char *error;
    int   erroffset;
    pcre *re;
    int   rc;
    int   i;
    int   ovector[100];
    char *found = NULL;


    re = pcre_compile (regex,          /* the pattern */
                       PCRE_MULTILINE,
                       &error,         /* for error message */
                       &erroffset,     /* for error offset */
                       0);             /* use default character tables */
    if (!re)
    {
        //printf("pcre_compile failed (offset: %d), %s\n", erroffset, error);
        return NULL;
    }

    unsigned int offset = 0;
    unsigned int len    = strlen(text);
    while (offset < len && (rc = pcre_exec(re, 0, text, len, offset, 0, ovector, sizeof(ovector))) >= 0)
    {
        //printf("while\n");
        for(int i = 0; i < rc; ++i)
        {
			//printf("%2d - %.*s\n",i,ovector[2*i+1] - ovector[2*i],text + ovector[2*i]);
			if (i == group) {
				
				int lencpy = ovector[2*i+1] - ovector[2*i];
				found = (char *)malloc((lencpy+1) *sizeof(char));
				char* full_part = (char*)text + ovector[2*i];
				memcpy(found, full_part,lencpy);
				memset(found+lencpy,'\0',1);
				//printf("%s",found);
				break;
			}
        }
		
		if (found != NULL) {
			break;
		}
        offset = ovector[1];
    }
     pcre_free(re);
    
    return found;
}

void strupper(char *s) {
	char *p;
	for (p = s; *p != '\0'; p++) {
		*p = (char) toupper(*p);
	}
}
void strlower(char *s){
	char  *p;
	for (p = s; *p != '\0'; p++) {
		*p = (char) tolower(*p);
	}
}