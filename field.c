#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gumbo.h>
#include "gumbo_util.h"
#include "field.h"

void retrieve_form_field_properties(RECForm_field *form_field, GumboVector *form_field_attrs) {
	char *form_field_name = get_tag_attribute(form_field_attrs,"NAME");
	if (form_field_name != NULL) {
		form_field->name = malloc((strlen(form_field_name) + 1) * sizeof(char));
		strcpy(form_field->name,form_field_name);
	}

	char *form_field_id = get_tag_attribute(form_field_attrs,"ID");
	if (form_field_id != NULL) {
		form_field->id = malloc((strlen(form_field_id) + 1) * sizeof(char));
		strcpy(form_field->id,form_field_id);
	}

	char *form_field_value = get_tag_attribute(form_field_attrs,"VALUE");
	if (form_field_value != NULL) {
		form_field->value = malloc((strlen(form_field_value) + 1) * sizeof(char));
		strcpy(form_field->id,form_field_value);
	}
}

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
	printf("1");
	RECForm_field     *last;
	RECForm_field     *new_item;
	printf("2");
	new_item = malloc(sizeof(RECForm_field));
	if(!new_item)
		return NULL;
	printf("3");
	new_item->next = NULL;

	if (field->name) {
		new_item->name = (char*)malloc((strlen(field->name) + 1) *sizeof(char));
		strcpy(new_item->name,field->name);
	}
	printf("4");
	if (field->id) {
		new_item->id = (char*)malloc((strlen(field->id) + 1) *sizeof(char));
		strcpy(new_item->id,field->id);
	}
	printf("5");
	if (field->value) {
		new_item->value = (char*)malloc((strlen(field->value) + 1) *sizeof(char));
		strcpy(new_item->value,field->value);
	}
	printf("6");
	//new->type =

	/* if this is the first item, then new_item *is* the list */
	if(!fields)
		return new_item;
	printf("7");
	last = field_get_last(fields);
	last->next = new_item;
	printf("8");
	return fields;
}


RECForm_field *retrieve_form_fields(RECForm_field *recform_field, const GumboVector* children) {
	int i;
	for (i = 0; i < children->length; ++i) {
		GumboNode* child = children->data[i];
		if (child->type == GUMBO_NODE_ELEMENT) {
			char *tagname =(char*)gumbo_normalized_tagname(child->v.element.tag);
			printf("%s\n",tagname);
			printf("filho: %d\n",i);
			if (child->v.element.tag == GUMBO_TAG_INPUT) {
				printf("depois é field\n");
				printf("\té field\n");

				RECForm_field *form_field = (RECForm_field*) malloc(sizeof(RECForm_field));
				form_field->id = NULL;
				form_field->name = NULL;
				form_field->value = NULL;
				printf("setou null\n");

				GumboVector *form_field_attrs = &child->v.element.attributes;
				printf("atributos\n");
				//GET INPUT PROPERTIES
				retrieve_form_field_properties(form_field, form_field_attrs);
				recform_field = field_add_nodup(recform_field, form_field);
				printf("\nadd no dup\n");

			}
			if (child->v.element.children.length > 0) {
				recform_field = retrieve_form_fields(recform_field,&child->v.element.children);
			}
		}
	}
	return recform_field;
}

