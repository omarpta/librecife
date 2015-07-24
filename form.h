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
#ifndef HEADER_RECIFE_FORM_H
#define HEADER_RECIFE_FORM_H
 
#include <gumbo.h>
#include "field.h"

typedef enum {
    RECIFE_MULTIPART_FORM_DATA,
    RECIFE_TEXT_PLAIN,
    RECIFE_APPLICATION_X_WWW_FORM_URLENCODED
} form_enc_type;

typedef enum {
    RECIFE_POST,
    RECIFE_GET,
    RECIFE_PUT,
	RECIFE_DELETE,
	RECIFE_OPTION,
} form_method;


/*
 * RECForm
 * Linked list struct to store document forms
 */
typedef struct recform {
	char *id;
    char *name;
	char *action;
	form_method method;
    form_enc_type type;
    
	RECForm_field *fields;
    struct recform *next;
} RECForm;

typedef void RECIFE_FORM;

static void retrieve_form_properties(RECForm *form, GumboVector *form_attrs);



RECForm * retrieve_html_forms(RECForm *recform, const GumboVector* children, char *name);

#endif /* HEADER_RECIFE_FORM_H */
