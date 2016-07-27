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


static size_t _remove_separators(charptr destination, const charptr source, size_t length) {
    size_t new_length = 0;

    for (size_t i = 0; i < length; i++) {
        if (source[i] == '_') {
            continue;
        } else {
            destination[new_length] = source[i];
            new_length++;
        }
    }

    return new_length;
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
        } else {    // this is last `_segment`
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


static void _init_number(Number number, const charptr source, size_t length) {
    charptr segment;
    size_t  int_pos = 0;    // position in `source`, where integer part begins
    size_t  fract_pos = 0;  // position in `source`, where fraction part begins
    size_t  exp_pos = 0;    // position in `source`, where exponent part begins

    if (source[int_pos] == '~') {
        number->_is_approximate = True;
        int_pos++;
    } else {
        number->_is_approximate = False;
    }

    if (source[int_pos] == '+') {
        number->_is_negative = False;
        int_pos++;
    } else if (source[int_pos] == '-') {
        number->_is_negative = True;
        int_pos++;
    } else {
        number->_is_negative = False;
    }

    for (size_t i = int_pos; i < length; i++) {
        if (source[i] == '0') {
            int_pos++;
        } else if (source[i] == '.') {      // 0.nnnnnn
            fract_pos = i + 1;  // fration part begins here
            int_pos--;
            break;
        } else {
            break;
        }
    } if (int_pos == length) {
        /*
         * `source` can be:
         * -- "000000000...0"
         * -- "+00000000...0"
         * -- "-00000000...0"
         * -- "~00000000...0"
         * -- "~+0000000...0"
         * -- "~-0000000...0"
         *
         * These variants must be reduce to "0" or "~0"
         */
        number->_is_negative = False;
        number->_is_exp_negative = False;
        number->_has_period = False;
        number->_has_endless_fract = False;

        segment = (charptr) malloc(sizeof(char) * 5);

        segment[0] = '0';
        segment[1] = '0';
        segment[2] = '0';
        segment[3] = source[int_pos - 1];
        segment[4] = '\0';

        number->_integer->push_front(number->_integer, segment);

        return;
    }

    fract_pos = (!fract_pos) ? _has_fraction(source, int_pos, length - 1) : 0;

    // Now we can define the integer part, starting from the end of it.
    if (fract_pos) {    // if `source` has a fraction part
        /*
         * `int_pos`       - position in `source` where integer part begins
         * `fract_pos`     - position in `source` where fraction part begins
         * `fract_pos - 1` - position in which `source[j - 1]` == '.'
         * `fract_pos - 2` - position, where integer part of `number` ends
         */
        _init_integer_part(number, source, int_pos, fract_pos - 2);
    } else {    // else the `number` hasn't fraction part
        /*
         * `int_pos`    - position in `source` where integer part begins
         * `length`     - length of `source`
         * `length - 1` - position in `source` where integer part ends
         */
        _init_integer_part(number, source, int_pos, length - 1);
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

    size_t  len = strlen(number_in_string);
    charptr num_in_str_without_sep = (charptr) malloc(sizeof(char) * len);
    size_t  new_len = _remove_separators(num_in_str_without_sep, number_in_string, len);

    /*
     * If `new_len` == 1, then `num_in_str_without_sep` holds only one digit (0-9),
     * without any extra characters ('+', '-', '~', and so on). Otherwise,
     * it's not a Number literal at all.
     *
     * If `new_len` == 2, then `num_in_str_without_sep` can contain extra characters
     * at the beginning: '+', '-' or '~'. Sequence "-0" must be reduce to the "0".
     *
     * If `new_len` >= 3, then `num_in_str_without_sep` can be any of forms:
     * -- integer:              "243"
     * -- with '+':             "+10"
     * -- negative:             "-56"
     * -- real:                 "0.1"
     * -- approximate:          "~12"
     * -- approx. and negative: "~-2"
     * -- scientific:           "2e5"
     *
     * If `new_len` >= 5, then `num_in_str_without_sep` can contain fraction part
     * with period: "0.(4)". Sequence "0.(0)" must be reduce to the "0".
     *
     * If `new_len` >= 6, then `num_in_str_without_sep` can contain endless fraction
     * part: "0.1...".
     *
     * `num_in_str_without_sep` can hold zeros at the beginning and at the end:
     * "04", "345.500", "-03". Sequence "-00000000" must be reduce to the "0".
     */
    _init_number(self, num_in_str_without_sep, new_len);

    free(num_in_str_without_sep);
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
