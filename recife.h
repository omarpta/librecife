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
#include <gumbo.h>
#include "form.h"







enum user_agents {
	RECIFE_CHROME_LINUX, 
	RECIFE_CHROME_ANDROID, 
	RECIFE_AVANT_BROWSER_WINDOWS, 
	RECIFE_OPERA_LINUX, 
	RECIFE_INTERNET_EXPLORER, 
	RECIFE_SAFARI_MACOS
};
typedef enum user_agents user_agent;

enum navitate_codes {RECIFE_COMPLETE, RECIFE_ERROR};
typedef enum navitate_codes navigate_code;

typedef void RECIFE;

extern RECIFE *recife_init_with_agent(user_agent agent);

extern RECIFE *recife_init(void);

extern navigate_code recife_navigate(RECIFE *recife, const char* url);

extern char* recife_get_content(RECIFE *recife);

extern void recife_free(RECIFE *recife);

void process_html_parsing(RECIFE *recife);

extern RECForm *recife_form_by_name(RECIFE *recife, char *name);

char * get_tag_attribute(GumboVector *tag_attrs, char *attr_name);

#endif /* HEADER_RECIFE_H */