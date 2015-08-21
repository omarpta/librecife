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

#include "gumbo_util.h"


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
