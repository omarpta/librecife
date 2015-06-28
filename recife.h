/***************************************************************************
 *        
 *         _____  ______ _____ _____ ______ ______ 
 *        |  __ \|  ____/ ____|_   _|  ____|  ____|
 *        | |__) | |__ | |      | | | |__  | |__   
 *        |  _  /|  __|| |      | | |  __| |  __|  
 *        | | \ \| |___| |____ _| |_| |    | |____ 
 *        |_|  \_\______\_____|_____|_|    |______|
 *
 *
 * Request Creation Interface - RECIFE
 * Copyright (C) 2015, Omar Torres, <omar@analista.io>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://librecife.analista.io/copyright.html .
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
 
#ifndef HEADER_RECIFE_H
#define HEADER_RECIFE_H

#include <curl/curl.h>
#include <nvlist.h> 


typedef enum {
    MULTIPART_FORM_DATA,
    TEXT_PLAIN,
    APPLICATION_X_WWW_FORM_URLENCODED
} form_enc_type;

typedef enum {
    TEXT, 
    SELECT, 
    SUBMIT, 
    TEXTAREA, 
    PASSWORD, 
    BUTTON,
    RADION,
    CHECKBOX,
    COLOR,
    DATE,
    DATETIME,
    DATETIME_LOCAL,
    EMAIL,
    MONTH,
    NUMBER,
    RANGE,
    SEARCH,
    TEL,
    TIME,
    URL,
    WEEK,
} input_type;

/*
 * RECForm_field
 * Linked list struct to store input fields
 */
typedef struct recform_field {
    char *id;
    char *name;
    char *value;
    input_type type;
    struct recform_field *next;
    
} RECForm_field;

/*
 * RECForm
 * Linked list struct to store document forms
 */
typedef struct recform {
    char *name;
	char *method;
	char *action;
    form_enc_type type;
    
	RECForm_field *fields;
    struct recform *next;
} RECForm;


enum user_agents {CHROME_LINUX, CHROME_ANDROID, AVANT_BROWSER_WINDOWS, OPERA_LINUX, INTERNET_EXPLORER, SAFARI_MACOS};
typedef enum user_agents user_agent;

enum navitate_codes {RECIFE_COMPLETE, RECIFE_ERROR};
typedef enum navitate_codes navigate_code;

typedef void RECIFE;

extern RECIFE *recife_init(user_agent agent);

extern navigate_code recife_navigate(RECIFE *recife, const char* url);

extern char* recife_get_content(RECIFE *recife);

extern void recife_free(RECIFE *recife);

void process_html_parsing(RECIFE *recife);

extern RECForm *recife_form_by_name(RECIFE *recife, char *name);

#endif /* HEADER_RECIFE_H */