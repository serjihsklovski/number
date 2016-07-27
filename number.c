#include <number.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


static size_t _has_fraction(const charptr source, size_t begin, size_t end) {
    size_t pos = 0;

    for (size_t i = begin; i < end; i++) {
        if (source[i] == '.') {
            pos = i + 1;
            break;
        }
    }

    return pos;
}


static void _init_integer_part(Number number, const charptr source, size_t begin, size_t end) {
    int     _end = (int) end;
    int     _begin = (int) begin;
    int     _length;
    int     _rem;
    charptr _segment;

    for (int i = _end; i >= _begin; i -= 4) {
        _segment = (charptr) malloc(sizeof(char) * 5);
        _length = i - _begin + 1;

        if (_length / 4 != 0) {
            _segment[0] = source[i - 3];
            _segment[1] = source[i - 2];
            _segment[2] = source[i - 1];
            _segment[3] = source[i];
        } else {    // this is last `segment`
            _segment[0] = '0';
            _rem = _length % 4;

            if (_rem > 1) {
                _segment[1] = (_rem == 3) ? source[i - 2] : '0';
                _segment[2] = source[i - 1];
            } else {
                _segment[1] = '0';
                _segment[2] = '0';
            }

            _segment[3] = source[i];
        }

        _segment[4] = '\0';

        number->_integer->push_front(number->_integer, _segment);
    }
}


static Bool _add_four_digits(charptr res, const charptr op_1, const charptr op_2, Bool last_overflow) {
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


static void _number_len_1(Number number, const charptr source) {
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

    charptr segment = (charptr) malloc(sizeof(char) * 5);

    segment[0] = '0';
    segment[1] = '0';
    segment[2] = '0';
    segment[3] = source[0];
    segment[4] = '\0';

    number->_integer->push_front(number->_integer, segment);
}


static void _number_len_2(Number number, const charptr source) {
    /*
     * If `len` == 2, then `number_in_string` can contain extra characters
     * at the beginning: '+', '-' or '~'. Sequence "-0"must be reduce to the "0".
     */

    number->_is_exp_negative = False;
    number->_has_period = False;
    number->_has_endless_fract = False;

    charptr segment = (charptr) malloc(sizeof(char) * 5);

    switch (source[0]) {
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


static void _number_len_3(Number number, const charptr source) {
    /*
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
     * Sequence "0.0" must be reduced to "0"
     */

    number->_is_exp_negative = False;
    number->_has_period = False;
    number->_has_endless_fract = False;

    charptr segment = (charptr) malloc(sizeof(char) * 5);

    size_t i = 0;   // position in `source`, where integer part begins
    size_t j = 0;   // position in `source`, where fraction part begins

    if (source[i] == '~') {
        number->_is_approximate = True;
        i++;
    }

    if (source[i] == '+') {
        number->_is_negative = False;
        i++;
    } else if (source[i] == '-') {
        number->_is_negative = True;
        i++;
    }

    for (size_t k = i; k < 3; k++) {
        //
    }
}


static void _number_len_4(Number number, const charptr source) {
    //
}


static void _number_len_5(Number number, const charptr source) {
    //
}


static void _number_len_6_and_more(Number number, const charptr source, size_t len) {
    charptr segment;
    size_t  i = 0;      // position in `source`, where integer part begins
    size_t  j = 0;      // position in `source`, where fraction part begins

    if (source[i] == '~') {
        number->_is_approximate = True;
        i++;
    }

    if (source[i] == '+') {
        number->_is_negative = False;
        i++;
    } else if (source[i] == '-') {
        number->_is_negative = True;
        i++;
    }

    for (size_t k = i; k < len; k++) {
        if (source[k] == '0') {
            i++;
        } else if (source[k] == '.') {      // 0.nnnnnn
            j = k + 1;  // fration part begins here
            i--;
            break;
        } else {
            break;
        }
    } if (i == len) {
        /* `source` can be:
         * -- "000000000...0"
         * -- "+00000000...0"
         * -- "-00000000...0"
         * -- "~00000000...0"
         * -- "~+0000000...0"
         * -- "~-0000000...0"
         *
         * These variants must be reduce to "0" or "~0"
         */
        number->_is_exp_negative = False;
        number->_has_period = False;
        number->_has_endless_fract = False;

        segment = (charptr) malloc(sizeof(char) * 5);

        segment[0] = '0';
        segment[1] = '0';
        segment[2] = '0';
        segment[3] = source[i - 1];
        segment[4] = '\0';

        number->_integer->push_front(number->_integer, segment);

        return;
    }

    j = _has_fraction(source, i, len - 1);

    // Now we can define the integer part, starting from the end of it.
    if (j) {    // if `source` has a fraction part
        /*
         * `i`     - position in `source` where integer part begins
         * `j`     - position in `source` where fraction part begins
         * `j - 1` - position in which `source[j - 1]` == '.'
         * `j - 2` - position, where integer part of number ends
         */
        _init_integer_part(number, source, i, j - 2);
    } else {    // else the `number` hasn't fraction part
        /*
         * `i` - position in `source` where integer part begins
         * `len` - length of `source`
         * `len - 1` - position in `source` where integer part ends
         */
        _init_integer_part(number, source, i, len - 1);
    }
}


constructor_(Number)(const charptr number_in_string) {
    /*
     * Number constructor does not check whether parameter number_in_string
     * is entered correctly or not. This task falls to external code.
     * Tt is assumed that `number_in_string` contains correct representation
     * of Number literal.
     */

    new_self_(Number);

    self->_integer = new_(List_charptr)();
    self->_fraction = new_(List_charptr)();
    self->_period = new_(List_charptr)();
    self->_exponent = new_(List_charptr)();

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

    delete_List_charptr(self->_integer);
    self->_integer = NULL;
    delete_List_charptr(self->_fraction);
    self->_fraction = NULL;
    delete_List_charptr(self->_period);
    self->_period = NULL;
    delete_List_charptr(self->_exponent);
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
