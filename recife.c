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
#include "recife.h"
#include "recife_util.h"
#include <stdlib.h>
#include <string.h>

        


static void init_rec_content(RECcontent* s)
{
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if(s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

static size_t writefunc(void* ptr, size_t size, size_t nmemb, RECcontent* s)
{
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if(s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}

static const char *get_user_agent(user_agent agent) {
    switch (agent) {
        case CHROME_LINUX:
            return "Linux - Mozilla/5.0 (Linux; Android 5.0; LG-D855 Build/LRX21R) AppleWebKit/537.36 (KHTML, "
						   "like Gecko) Chrome/43.0.2357.93 Mobile Safari/537.36";
            break;
            
        default:
            return NULL;
            break;
        
    }
    
}

static struct curl_slist* curl_slist_set_value(struct curl_slist* list, const char* data, const char* value)
{
    struct curl_slist* next;
    struct curl_slist* item;
    int found = 0;
    char* full_data;
    full_data = (char*)malloc((strlen(data) + strlen(value) + 2) * sizeof(char));
    strcpy(full_data, data);
    strcat(full_data, " ");
    strcat(full_data, value);

    if(list) {
		item = list;
		do {
			next = item->next;
			if(strstr(item->data, data) != NULL) {
				found = 1;
				free(item->data);
				item->data = (char*)realloc(item->data, sizeof(full_data));
				strcpy(item->data, full_data);
				break;
			}
			item = next;
		} while(next);
    }

    if(!found) {
		list = curl_slist_append(list, full_data);
    }
    free(full_data);
    return list;
}

static REC *get_recife(RECIFE *recife) {
    return (REC*) recife;
}

static char *get_host(const char* url) {
    get_regex_group(url, "https?:\\/\\/(.*?)(\\/)", 1);
    
}



RECIFE *recife_init(user_agent agent) {
    //#### BEGIN DEBUG SPACE ####
    printf("%s", get_host("https://portal.fitec.org.br/corpore.net/teste/Login.aspx"));
    
    exit(0);
    
    
    //#### END DEBUG SPACE ####
    
    
    REC *rec;
    
    rec = (REC*) malloc(sizeof(REC));
    if (!rec) 
        return NULL;
    
    rec->curl = curl_easy_init();
    
    
	init_rec_content(&rec->content);
    rec->curl_headers = NULL;
    
    rec->curl_headers = curl_slist_set_value(rec->curl_headers, "Accept:", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    rec->curl_headers = curl_slist_set_value(rec->curl_headers, "Host:", "portal.fitec.org.br");
    rec->curl_headers = curl_slist_set_value(rec->curl_headers, "Connection:", "keep-alive");
    
    curl_easy_setopt(rec->curl, CURLOPT_USERAGENT, get_user_agent(agent));
    curl_easy_setopt(rec->curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(rec->curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(rec->curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(rec->curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(rec->curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(rec->curl, CURLOPT_WRITEDATA, &rec->content);
    curl_easy_setopt(rec->curl, CURLOPT_COOKIEJAR, "recife_session_id");
    curl_easy_setopt(rec->curl, CURLOPT_HTTPHEADER, rec->curl_headers);
		
    
    return rec;

}

navigate_code recife_navigate(RECIFE *recife, const char* url) {
    REC *rec = get_recife(recife);
    curl_easy_setopt(rec->curl, CURLOPT_URL, url);
    rec->curl_res = curl_easy_perform(rec->curl);
    if(rec->curl_res != CURLE_OK) {
		return RECIFE_ERROR;
    } else {
        return RECIFE_COMPLETE;
    }
}


char* recife_get_content(RECIFE *recife) {
    REC *rec = get_recife(recife);
    if (rec->content.ptr == NULL)
        return NULL;
        
    return rec->content.ptr;
}

void recife_free(RECIFE *recife) {
    REC *rec = get_recife(recife);
    curl_easy_cleanup(rec->curl);
    curl_slist_free_all(rec->curl_headers);
    free(rec);

}