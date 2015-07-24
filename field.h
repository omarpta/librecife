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
#ifndef HEADER_RECIFE_FIELD_H
#define HEADER_RECIFE_FIELD_H

#include <gumbo.h>

typedef enum {
    RECIFE_TEXT,
    RECIFE_SELECT,
    RECIFE_SUBMIT,
    RECIFE_TEXTAREA,
    RECIFE_PASSWORD,
    RECIFE_BUTTON,
    RECIFE_RADION,
    RECIFE_CHECKBOX,
    RECIFE_COLOR,
    RECIFE_DATE,
    RECIFE_DATETIME,
    RECIFE_DATETIME_LOCAL,
    RECIFE_EMAIL,
    RECIFE_MONTH,
    RECIFE_NUMBER,
    RECIFE_RANGE,
    RECIFE_SEARCH,
    RECIFE_TEL,
    RECIFE_TIME,
    RECIFE_URL,
    RECIFE_WEEK,
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

static RECForm_field *retrieve_form_fields(RECForm_field *recform_field, const GumboVector* children);

#endif /* HEADER_RECIFE_FIELD_H */
