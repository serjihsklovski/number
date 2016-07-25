#ifndef NUMBER_H
#define NUMBER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/cclasses/cclasses.h"
#include "lib/cexception/CException.h"
#include "lib/template_list.h"
#include "error_codes.h"
#include "list_string.h"
#include <stdlib.h>


cclass_(Number) {
    /* fields */
    _Bool           _is_negative;
    List(String)    _integer;
    List(String)    _fraction;

    /* methods */
    method_def_(Number,     add,            Number) with_(Number other);
    method_def_(Number,     sub,            Number) with_(Number other);
    method_def_(Number,     mul,            Number) with_(Number other);
    method_def_(Number,     div,            Number) with_(Number other);
    method_def_(Number,     mod,            Number) with_(Number other);
    method_def_(Number,     pow,            Number) with_(Number other);

    method_def_(_Bool,      e,              Number) with_(Number other);
    method_def_(_Bool,      ne,             Number) with_(Number other);
    method_def_(_Bool,      lt,             Number) with_(Number other);
    method_def_(_Bool,      let,            Number) with_(Number other);
    method_def_(_Bool,      mt,             Number) with_(Number other);
    method_def_(_Bool,      met,            Number) with_(Number other);

    method_def_(_Bool,      is_negative,    Number) without_args;
    method_def_(_Bool,      is_integer,     Number) without_args;
    method_def_(Number,     integer,        Number) without_args;
    method_def_(Number,     fraction,       Number) without_args;
};

constructor_(Number)(const String number_in_string);
destructor_(Number);


#ifdef __cplusplus
}
#endif

#endif // NUMBER_H
