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
#include "stdlib.h"
#include "string.h"
#include "recife_util.h"
#include "form.h"

char * get_tag_attribute(GumboVector *tag_attrs, char *attr_name) {
	int j;
	for (j = 0; j < tag_attrs->length;++j) {
		GumboAttribute* att = tag_attrs->data[j];
		char *tag_attr_name = strdup(att->name);
		strupper(tag_attr_name);
		if (strcmp(tag_attr_name,attr_name) == 0) {
			return strdup(att->value);
		}
	}
	
	return NULL;
}

void retrieve_form_properties(RECForm *form, GumboVector *form_attrs) {
	char *form_name = get_tag_attribute(form_attrs,"NAME");
	if (form_name != NULL) {
		form->name = malloc((strlen(form_name) + 1) * sizeof(char));
		strcpy(form->name,form_name);
	}
	
	char *form_method = get_tag_attribute(form_attrs,"METHOD");
	if (form_method != NULL) {
		strupper(form_method);
		if (strstr(form_method,"POST") != NULL) {
			form->method = RECIFE_POST;
		} else {
			form->method = RECIFE_GET;
		}
	} else {
		form->method = RECIFE_GET;
	}
	
	char *form_action = get_tag_attribute(form_attrs,"ACTION");
	if (form_action != NULL) {
		form->action = malloc((strlen(form_action) + 1) * sizeof(char));
		strcpy(form->action,form_action);
	}
	
	char *form_enctype = get_tag_attribute(form_attrs,"ENCTYPE");
	if (form_enctype != NULL) {
		strupper(form_enctype);
		if (strstr(form_enctype,"APPLICATION/X-WWW-FORM-URLENCODED") != NULL) {
			form->type = RECIFE_APPLICATION_X_WWW_FORM_URLENCODED;
		} else if (strstr(form_enctype,"MULTIPART/FORM-DATA") != NULL) {
			form->type = RECIFE_MULTIPART_FORM_DATA;
		} else {
			form->type = RECIFE_TEXT_PLAIN;
		}
	}
	
	char *form_id = get_tag_attribute(form_attrs,"ID");
	if (form_id != NULL) {
		form->id = malloc((strlen(form_id) + 1) * sizeof(char));
		strcpy(form->id,form_id);
	}
}

RECForm *form_get_last(RECForm *list) {
  RECForm     *item;

  /* if caller passed us a NULL, return now */
  if(!list)
    return NULL;

  /* loop through to find the last item */
  item = list;
  while(item->next) {
    item = item->next;
  }
  return item;
}

RECForm *form_add_nodup(RECForm *forms, RECForm *form)
{
  RECForm     *last;
  RECForm     *new_item;

  new_item = malloc(sizeof(RECForm));
  if(!new_item)
    return NULL;

  new_item->next = NULL;
  
  if (form->name) {
	new_item->name = (char*)malloc((strlen(form->name) + 1) *sizeof(char));
	strcpy(new_item->name,form->name);
  }
  
  if (form->id) {
	new_item->id = (char*)malloc((strlen(form->id) + 1) *sizeof(char));
	strcpy(new_item->id,form->id);
  }
  if (form->action) {
	new_item->action = (char*)malloc((strlen(form->action) + 1) *sizeof(char));
	strcpy(new_item->action,form->action);
  }
  
  //new->type = 

  /* if this is the first item, then new_item *is* the list */
  if(!forms)
    return new_item;

  last = form_get_last(forms);
  last->next = new_item;
  
  return forms;
}

RECForm * retrieve_html_forms(RECForm *recform, const GumboVector* children, char *name) {
    //fprintf(stderr,"sons off: %s\n",name);
    //RECForm *form = (RECForm*)recform;
    for (int i = 0; i < children->length; ++i) {
        GumboNode* child = children->data[i];
        if (child->type == GUMBO_NODE_ELEMENT) {
			//RECForm * current_form = NULL;
            char *tagname =(char*)gumbo_normalized_tagname(child->v.element.tag);
			printf("%s",tagname);
            if (child->v.element.tag == GUMBO_TAG_FORM) {
				
				RECForm *form = (RECForm*) malloc(sizeof(RECForm));
				form->id = NULL;
				form->name = NULL;
				form->fields = NULL;
				form->action = NULL;
				form->type = RECIFE_TEXT_PLAIN;
				form->method = RECIFE_GET;
				GumboVector *form_attrs = &child->v.element.attributes;
				retrieve_form_properties(form, form_attrs);
				recform = form_add_nodup(recform, form);
				
            } else if (child->v.element.tag == GUMBO_TAG_INPUT) {
                //if (form != NULL) {
					//printf("Last form name: %s\n",form->name);
					//if (!form->fields) {
						//form->fields = (RECForm_field*) malloc(sizeof(RECForm_field))
						
						//if (form) {
//							RECForm *actual_form;
//							if (form) {
//								actual_form = form;
//							} else {
//								actual_form = form;
//							}
						//}
					//}
				//} else {
				//	printf("form nulo");
				//}
            }
            
            
            
            if (child->v.element.children.length > 0) {
                //fprintf(stderr,"-sons: %s:\n", tagname);
				
					//printf("retrieve using form\n ");
					//recform = retrieve_html_forms(recform,&child->v.element.children, tagname);
				
                //fprintf(stderr,"-----: %s\n",tagname);
            }
        }
    }
	return recform;
}