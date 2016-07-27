#include <number.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


static Bool _add_four_digits(String res, const String op_1, const String op_2, Bool last_overflow) {
    // annotation: memory for `res` has to be allocated!
    const int   MAX_COUNT = 6;
    char        buf[MAX_COUNT];
    _Bool       overflow_flg = False;

    itoa(atoi(op_1) + atoi(op_2) + last_overflow, buf, 10);     // 10 - decimal radix

    /*
     * minimum: 0 + 0 + 0 == 0           => `buf` == {'0', '\0'}                     => 2 characters
     * maximum: 9999 + 9999 + 1 == 19999 => `buf` == {'1', '9', '9', '9', '9', '\0'} => 6 characters
     */

    size_t len = strnlen(buf, MAX_COUNT);

    if (len < 5) {
        for (unsigned int i = 0; i < 4 - len; i++) {
            strncat(res, "0", MAX_COUNT);
        }

        strncat(res, buf, MAX_COUNT);
    } else {
        overflow_flg = True;
        strncpy(res, buf + 1, MAX_COUNT);
    }

    return overflow_flg;
}


static void _number_len_1(Number number, const String source) {
    /*
     * If `len` == 1, then `number_in_string` holds only one digit (0-9),
     * without any extra characters ('+', '-', '~', and so on). Otherwise,
     * it's not a Number literal at all.
     */

    number->_is_negative = False;
    number->_is_approximate = False;
    number->_is_exp_negative = False;
    number->_has_period = False;
    number->_has_endless_fract = False;

    String segment = (String) malloc(sizeof(char) * 5);

    segment[0] = '0';
    segment[1] = '0';
    segment[2] = '0';
    segment[3] = source[0];
    segment[4] = '\0';

    number->_integer->push_front(number->_integer, segment);
}


static void _number_len_2(Number number, const String source) {
    /*
     * If `len` == 2, then `number_in_string` can contain extra characters
     * at the beginning: '+', '-' or '~'. Sequence "-0"must be reduce to the "0".
     */

    number->_is_exp_negative = False;
    number->_has_period = False;
    number->_has_endless_fract = False;

    String segment = (String) malloc(sizeof(char) * 5);

    switch (source[0]) {
        case '0':   // `source` == "0n" => "n"
        case '+':
            number->_is_negative = False;
            number->_is_approximate = False;
            segment[0] = '0';
            segment[1] = '0';
            segment[2] = '0';
            segment[3] = source[1];
            break;

        case '-':
            number->_is_negative = (source[1] == '0') ? False : True;   // if `source` == "-0" => "0"
            number->_is_approximate = False;
            segment[0] = '0';
            segment[1] = '0';
            segment[2] = '0';
            segment[3] = source[1];
            break;

        case '~':
            number->_is_negative = False;
            number->_is_approximate = True;
            segment[0] = '0';
            segment[1] = '0';
            segment[2] = '0';
            segment[3] = source[1];
            break;

        default:    // `source` == "10".."99"
            number->_is_negative = False;
            number->_is_approximate = False;
            segment[0] = '0';
            segment[1] = '0';
            segment[2] = source[0];
            segment[3] = source[1];
            break;
    }

    segment[4] = '\0';

    number->_integer->push_front(number->_integer, segment);
}


static void _number_len_3(Number number, const String source) {
    //
}


static void _number_len_4(Number number, const String source) {
    //
}


static void _number_len_5(Number number, const String source) {
    //
}


static void _number_len_6_and_more(Number number, const String source, size_t len) {
    //
}


constructor_(Number)(const String number_in_string) {
    /*
     * Number constructor does not check whether parameter number_in_string
     * is entered correctly or not. This task falls to external code.
     * Tt is assumed that `number_in_string` contains correct representation
     * of Number literal.
     */

    new_self_(Number);

    self->_integer = new_(List_String)();
    self->_fraction = new_(List_String)();
    self->_period = new_(List_String)();
    self->_exponent = new_(List_String)();

    size_t len = strlen(number_in_string);

    /*
     * If `len` == 1, then `number_in_string` holds only one digit (0-9),
     * without any extra characters ('+', '-', '~', and so on). Otherwise,
     * it's not a Number literal at all.
     *
     * If `len` == 2, then `number_in_string` can contain extra characters
     * at the beginning: '+', '-' or '~'. Sequence "-0"must be reduce to the "0".
     *
     * If `len` >= 3, then `number_in_string` can be any of forms:
     * -- integer:              "243"
     * -- with '+':             "+10"
     * -- negative:             "-56"
     * -- real:                 "0.1"
     * -- approximate:          "~12"
     * -- approx. and negative: "~-2"
     * -- scientific:           "2e5"
     * -- with separator:       "1_0"
     *
     * If `len` >= 5, then `number_in_string` can contain fraction part
     * with period: "0.(4)". Sequence "0.(0)" must be reduce to the "0".
     *
     * If `len` >= 6, then `number_in_string` can contain endless fraction
     * part: "0.1...".
     *
     * `number_in_string` can hold zeros at the beginning and at the end:
     * "04", "345.500", "-03". Sequence "-00000000" must be reduce to the "0".
     */
    switch (len) {
        case 1u:
            _number_len_1(self, number_in_string);
            break;

        case 2u:
            _number_len_2(self, number_in_string);
            break;

        case 3u:
            _number_len_3(self, number_in_string);
            break;

        case 4u:
            _number_len_4(self, number_in_string);
            break;

        case 5u:
            _number_len_5(self, number_in_string);
            break;

        default:
            _number_len_6_and_more(self, number_in_string, len);
            break;
    }

    return self;
}


destructor_(Number) {
    while (!self->_integer->is_empty(self->_integer)) {
        free(self->_integer->pop_back(self->_integer));
    }

    while (!self->_fraction->is_empty(self->_fraction)) {
        free(self->_fraction->pop_back(self->_fraction));
    }

    while (!self->_period->is_empty(self->_period)) {
        free(self->_period->pop_back(self->_period));
    }

    while (!self->_exponent->is_empty(self->_exponent)) {
        free(self->_exponent->pop_back(self->_exponent));
    }

    delete_List_String(self->_integer);
    self->_integer = NULL;
    delete_List_String(self->_fraction);
    self->_fraction = NULL;
    delete_List_String(self->_period);
    self->_period = NULL;
    delete_List_String(self->_exponent);
    self->_exponent = NULL;

    free(self);
    self = NULL;
}


void print_number(Number number) {
    if (number->_is_approximate) {
        printf("~");
    }

    if (number->_is_negative) {
        printf("-");
    }

    for (size_t i = 0; i < number->_integer->_size; i++) {
        printf("%s", number->_integer->at(number->_integer, i));
    }

    if (!number->_fraction->is_empty) {
        printf(".");

        for (size_t i = 0; i < number->_fraction->_size; i++) {
            printf("%s", number->_fraction->at(number->_fraction, i));
        }
    }
}
