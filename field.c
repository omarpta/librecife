#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "field.h"



RECForm_field *field_get_last(RECForm_field *list) {
  RECForm_field     *item;

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

RECForm_field *field_add_nodup(RECForm_field *fields, RECForm_field *field)
{
  RECForm_field     *last;
  RECForm_field     *new_item;

  new_item = malloc(sizeof(RECForm_field));
  if(!new_item)
    return NULL;

  new_item->next = NULL;

  if (field->name) {
	new_item->name = (char*)malloc((strlen(field->name) + 1) *sizeof(char));
	strcpy(new_item->name,field->name);
  }

  if (field->id) {
	new_item->id = (char*)malloc((strlen(field->id) + 1) *sizeof(char));
	strcpy(new_item->id,field->id);
  }
  if (field->value) {
	new_item->value = (char*)malloc((strlen(field->value) + 1) *sizeof(char));
	strcpy(new_item->value,field->value);
  }

  //new->type =

  /* if this is the first item, then new_item *is* the list */
  if(!fields)
    return new_item;

  last = field_get_last(fields);
  last->next = new_item;

  return fields;
}


RECForm_field *retrieve_form_fields(RECForm_field *recform_field, const GumboVector* children) {
	int i;
	for (i = 0; i < children->length; ++i) {
		GumboNode* child = children->data[i];
		if (child->type == GUMBO_NODE_ELEMENT) {
			char *tagname =(char*)gumbo_normalized_tagname(child->v.element.tag);
			printf("%s\n",tagname);
			//printf("filho: %d\n",i);
			if (child->v.element.tag == GUMBO_TAG_INPUT) {
				printf("\t#é field#\n");
				RECForm_field *form_field = (RECForm_field*) malloc(sizeof(RECForm_field));
				form_field->id = NULL;
				form_field->name = NULL;
				form_field->value = NULL;


				GumboVector *form_field_attrs = &child->v.element.attributes;
				//GET INPUT PROPERTIES
				//retrieve_form_properties(form, form_attrs);
				recform_field = field_add_nodup(recform_field, form_field);

			}
			if (child->v.element.children.length > 0) {
				recform_field = retrieve_form_fields(recform_field,&child->v.element.children);
			}
		}
	}
	return recform_field;
}

