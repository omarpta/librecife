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

typedef struct recife_form {
	char *method;
	char *action;
	char *fields;
	char *buttons;
} RECForm;

typedef struct recife_user_agent {
	char *name;
	char *version;
	char *os_name;
} RECUser_agent;

typedef struct recife {
	CURL *curl;
	CURLcode curl_res;
	nvlist *form_fields;
	char *content;
	RECIFE_user_agent *user_agent;
	
} REC;

#endif /* HEADER_RECIFE_H */