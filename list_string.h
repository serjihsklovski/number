#ifndef LIST_STRING_H
#define LIST_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/template_list.h"

typedef char* String;

TemplateList(String)

#ifdef __cplusplus
}
#endif

#endif // LIST_STRING_H
