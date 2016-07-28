#ifndef NUMBER_H
#define NUMBER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/cclasses/cclasses.h"
#include "lib/cexception/CException.h"
#include "lib/template_list.h"
#include "error_codes.h"
#include "list_charptr.h"
#include <stdlib.h>


typedef enum Bool {
    False,
    True
} Bool;


cclass_(Number) {
    /* fields */
    Bool            _is_negative;
    Bool            _is_approximate;
    Bool            _is_exp_negative;
    Bool            _has_endless_fract;
    List(charptr)   _integer;
    List(charptr)   _fraction;
    List(charptr)   _period;
    List(charptr)   _exponent;

    /* methods */
    method_def_(Number,     add,            Number) with_(Number other);
    method_def_(Number,     sub,            Number) with_(Number other);
    method_def_(Number,     mul,            Number) with_(Number other);
    method_def_(Number,     div,            Number) with_(Number other);
    method_def_(Number,     mod,            Number) with_(Number other);
    method_def_(Number,     pow,            Number) with_(Number other);

    method_def_(Bool,       e,              Number) with_(Number other);
    method_def_(Bool,       ne,             Number) with_(Number other);
    method_def_(Bool,       lt,             Number) with_(Number other);
    method_def_(Bool,       let,            Number) with_(Number other);
    method_def_(Bool,       mt,             Number) with_(Number other);
    method_def_(Bool,       met,            Number) with_(Number other);

    method_def_(Bool,       is_negative,    Number) without_args;
    method_def_(Bool,       is_integer,     Number) without_args;
    method_def_(Number,     integer,        Number) without_args;
    method_def_(Number,     fraction,       Number) without_args;
};

constructor_(Number)(const charptr number_in_string);
destructor_(Number);

void print_number(Number number);


#ifdef __cplusplus
}
#endif

#endif // NUMBER_H
