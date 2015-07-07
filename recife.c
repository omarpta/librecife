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
#include <assert.h>
#include <form.h>


typedef struct {
    char* ptr;
    size_t len;
} RECcontent;
        
typedef struct {
	CURL *curl;
	CURLcode curl_res;
	char *user_agent;
    nvlist *headers;
    struct curl_slist* curl_headers;
	char *host;
    char *referer;
    RECcontent content;
    RECForm *forms;
    const GumboNode *doc;
} REC;

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
		case RECIFE_AVANT_BROWSER_WINDOWS:
			return "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; Avant Browser; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0; .NET CLR 3.0.04506; .NET CLR 3.5.21022; InfoPath.2)";
			break;
		case RECIFE_SAFARI_MACOS:
			return "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/7046A194A";
			break;
		case RECIFE_OPERA_LINUX:
			return "Opera/9.80 (X11; Linux i686; Ubuntu/14.10) Presto/2.12.388 Version/12.16";
			break;
		case RECIFE_CHROME_LINUX:
			return "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.81 Safari/537.36";
			break;
		case RECIFE_INTERNET_EXPLORER:
			return "Mozilla/5.0 (compatible, MSIE 11, Windows NT 6.3; Trident/7.0;  rv:11.0) like Gecko";
			break;
        case RECIFE_CHROME_ANDROID:
            return "Linux - Mozilla/5.0 (Linux; Android 5.0; LG-D855 Build/LRX21R) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.93 Mobile Safari/537.36";
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
	if (strstr(url, "http://") != NULL || strstr(url, "https://") != NULL) {
		return get_regex_group(url, "https?:\\/\\/(.*?)(\\/)", 1);
	} else if (strstr(url, "/") != NULL) {
		return get_regex_group(url, "(.*?)(\\/)", 1);
	} else {
		char *urldup = strdup(url);
		return urldup;
	}
}

static nvlist *init_rec_headers(nvlist *headers) {
    headers = nvlist_set(headers,"Accept","text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    headers = nvlist_set(headers,"Connection","keep-alive");
    return headers;
}

struct curl_slist* load_rec_headers(nvlist *headers) {
    //TODO: VEFIFY POSSIBLE USE OF SLIT_DELETE METHOD HERE TO AVOID USELESS MEMORY ALLOCATION
    nvlist *item;
    nvlist *next;
    struct curl_slist* curl_headers = NULL;
    item = headers;
    do {
        next = item->next;
        char *header_name = (char*) malloc((strlen(item->data->name) + 2) * sizeof(char));
        strcpy(header_name,item->data->name);
        strcat(header_name,":");
        
        curl_headers = curl_slist_set_value(curl_headers, header_name, item->data->value);
        
        item = next;
    } while (next);
    
    return curl_headers;
}

RECIFE *recife_init_with_agent(user_agent agent) {
    //#### BEGIN DEBUG SPACE ####

	//exit(0);

    //#### END DEBUG SPACE ####
    
    
    REC *rec;
    
    rec = (REC*) malloc(sizeof(REC));
    if (!rec) 
        return NULL;
    
    rec->curl = curl_easy_init();
    init_rec_content(&rec->content);
    rec->curl_headers = NULL;
    rec->headers = init_rec_headers(rec->headers);
     
    rec->curl_headers = load_rec_headers(rec->headers);

    
    curl_easy_setopt(rec->curl, CURLOPT_USERAGENT, get_user_agent(agent));
    curl_easy_setopt(rec->curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(rec->curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(rec->curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(rec->curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(rec->curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(rec->curl, CURLOPT_WRITEDATA, &rec->content);
    curl_easy_setopt(rec->curl, CURLOPT_COOKIEJAR, "recife_session_id");
    
    return rec;

}

RECIFE *recife_init(void) {
	return recife_init_with_agent(RECIFE_CHROME_LINUX);
}



//TODO: Create function to parser forms and create linked list and then create 
//another function to load inputs for the form
//Use add_nodup as template





void process_html_parsing(RECIFE *recife) {
    REC *rec = get_recife(recife);
    if (rec->content.ptr) {
        //GumboOutput* output = gumbo_parse_with_options(&kGumboDefaultOptions, html_source, strlen(html_source));
        
        GumboOutput* output = gumbo_parse(rec->content.ptr);
        rec->doc = (GumboNode*)malloc(sizeof(GumboNode));
        
        rec->doc = output->root;
        if (rec->doc->type == GUMBO_NODE_ELEMENT) {
            if (rec->doc->v.element.children.length > 0) {
                rec->forms = retrieve_html_forms(rec->forms,&rec->doc->v.element.children, "HTML");
				printf("antes\n");
				if (rec->forms) {
					RECForm *item;
					RECForm *next;
					
					item =  rec->forms;
				
					do {
						next = item->next;
						if (item->name) {
							printf("FOMULARIO: %s\n",item->name);
						}
						item = next;
					} while(next);
				}
            }
        }
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
}

navigate_code recife_navigate(RECIFE *recife, const char* url) {
    REC *rec = get_recife(recife);
	//TODO: Verify if referer is the host or the url
    if (rec->referer != NULL) {
        printf("referer setted\n");
        char *referer = strdup(rec->host);
        rec->headers = nvlist_set(rec->headers,"Referer",referer);
        nvlist_view(rec->headers);
    }
    
	char *host = get_host(url);
	rec->host = host;
    rec->headers = nvlist_set(rec->headers,"Host",rec->host);
	rec->curl_headers = load_rec_headers(rec->headers);
	curl_easy_setopt(rec->curl, CURLOPT_HTTPHEADER, rec->curl_headers);
    free(rec->content.ptr);
    init_rec_content(&rec->content);
    
    curl_easy_setopt(rec->curl, CURLOPT_URL, url);
    rec->curl_res = curl_easy_perform(rec->curl);
    if(rec->curl_res != CURLE_OK) {
		return RECIFE_ERROR;
    } else {
        rec->referer = (char*) malloc((strlen(url) + 1) * sizeof(char));
        strcpy(rec->referer,url);
        
        process_html_parsing(rec);
		
        
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
    curl_slist_free_all(rec->curl_headers);
    curl_easy_cleanup(rec->curl);    
	nvlist_free_all(rec->headers);
    free(rec->referer);
    free(rec->content.ptr);    
	free(rec->host);
    free(rec);
}



RECForm *recife_form_by_name(RECIFE *recife,char *name) {
    REC *rec = get_recife(recife);
    
    
    return  NULL;
}