#include <number.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


static Bool _has_fraction_and_exponent(const charptr    source,
                                       size_t           begin,
                                       size_t           end,
                                       size_t*          fract_pos,
                                       size_t*          exp_pos)
{
    *fract_pos = 0u;
    *exp_pos = 0u;

    size_t i;

    for (i = begin; i < end; i++) {
        if (source[i] == '.') {
            *fract_pos = i + 1;
            break;
        }
    } for (i = *fract_pos + 1; i < end; i++) {
        if (source[i] == 'e' || source[i] == 'E') {
            *exp_pos = i + 1;
            break;
        }
    }

    return *fract_pos && *exp_pos;
}


static size_t _has_period(const charptr source, size_t begin, size_t end) {
    size_t pos = 0;

    for (size_t i = begin; i < end; i++) {
        if (source[i] == '(') {
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

    destination[new_length] = '\0';

    return new_length;
}


static size_t _remove_zeros_begin(const charptr source, size_t begin, size_t end) {
    for (size_t i = begin; i < end; i++) {
        if (source[i] == '0') {
            begin++;
        } else {
            break;
        }
    }

    return begin;
}


static size_t _remove_zeros_end(const charptr source, size_t begin, size_t end) {
    for (size_t i = end - 1; i > begin; i--) {
        if (source[i] == '0') {
            end--;
        } else {
            break;
        }
    }

    return end;
}


static size_t _init_part(List_charptr lst, const charptr source, size_t begin, size_t end) {
    charptr segment;
    size_t  length = end - begin;
    size_t  rem = length % 4;
    size_t  ghost_zeros = 0;    // this is a count of beginning zeros in 1st segment, that won't be displayed

    if (rem) {
        segment = (charptr) malloc(sizeof(char) * 5);
        segment[0] = '0';
        ghost_zeros++;

        if (rem > 1) {
            if (rem > 2) {
                segment[1] = source[begin++];
            } else {
                segment[1] = '0';
                ghost_zeros++;
            }

            segment[2] = source[begin++];
        } else {
            segment[1] = '0';
            segment[2] = '0';
            ghost_zeros += 2;
        }

        segment[3] = source[begin++];
        segment[4] = '\0';

        lst->push_back(lst, segment);
    }

    for (size_t i = begin; i < end; i += 4) {
        segment = (charptr) malloc(sizeof(char) * 5);

        for (size_t j = 0; j < 4; j++) {
            segment[j] = source[i + j];
        }

        segment[4] = '\0';

        lst->push_back(lst, segment);
    }

    return ghost_zeros;     // it can be 0, 1, 2 or 3
}


static void _init_fraction_part(Number number, const charptr source, size_t begin, size_t end) {
    end = _remove_zeros_end(source, begin, end);
    number->_fract_ghost_zeros = _init_part(number->_fraction, source, begin, end);
}


static void _init_exponent_part(Number number, const charptr source, size_t begin, size_t end) {
    if (source[begin] == '+') {
        number->_is_exp_negative = False;
        begin++;
    } else if (source[begin] == '-') {
        number->_is_exp_negative = True;
        begin++;
    } else {
        number->_is_exp_negative = False;
    }

    number->_exp_ghost_zeros = _init_part(number->_exponent, source, begin, end);
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
    size_t int_pos = 0;     // position in `source`, where integer part begins
    size_t fract_pos = 0;   // position in `source`, where fraction part begins
    size_t per_pos = 0;     // position in `source`, where period of fraction begins
    size_t exp_pos = 0;     // position in `source`, where exponent part begins

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

    int_pos = _remove_zeros_begin(source, int_pos, length);     // "000000" => ""

    if (_has_fraction_and_exponent(source, int_pos, length - 1, &fract_pos, &exp_pos))
    {                               // `number` has integer, exponent, and fraction parts ("212.12414e124124")
        number->_int_ghost_zeros = _init_part(number->_integer, source, int_pos, fract_pos - 2);
        _init_fraction_part(number, source, fract_pos, exp_pos - 2);
        _init_exponent_part(number, source, exp_pos, length);
    } else if (exp_pos) {           // `number` has integer and exponent parts ("1231e5344")
        number->_int_ghost_zeros = _init_part(number->_integer, source, int_pos, exp_pos - 2);
        _init_exponent_part(number, source, exp_pos, length);
    } else if (fract_pos) {         // `number` has integer and fraction parts ("2355.5645", "2344.45(44345)", "123.342...")
        number->_is_exp_negative = False;

//        per_pos = _has_period(source, fract_pos, length - 2);

//        if (per_pos) {
//            _init_part(number->_period, source, per_pos, length - 1);
//        } else {
//            // fraction part is endless?
//        }

        number->_int_ghost_zeros = _init_part(number->_integer, source, int_pos, fract_pos - 1);
        _init_fraction_part(number, source, fract_pos, length);
    } else {                        // `number` has only integer part ("2353425678")
        number->_is_exp_negative = False;
        number->_has_endless_fract = False;
        number->_int_ghost_zeros = _init_part(number->_integer, source, int_pos, length);
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

    Node_charptr* node;
    size_t i;

    if (!number->_integer->is_empty(number->_integer)) {
        node = number->_integer->_head;
        i = 0;

        printf("%s", node->_data + number->_int_ghost_zeros);

        for (node = number->_integer->_head->_next; node != NULL; node = node->_next) {
            printf("%s", node->_data);
        }

        if (!number->_fraction->is_empty(number->_fraction)) {
            printf(".");

            node = number->_fraction->_head;
            i = 0;

            printf("%s", node->_data + number->_fract_ghost_zeros);

            for (node = number->_fraction->_head->_next; node != NULL; node = node->_next) {
                printf("%s", node->_data);
            }
        }
    } else if (!number->_fraction->is_empty(number->_fraction)) {
        printf("0.");

        node = number->_fraction->_head;
        i = 0;

        printf("%s", node->_data + number->_fract_ghost_zeros);

        for (node = number->_fraction->_head->_next; node != NULL; node = node->_next) {
            printf("%s", node->_data);
        }
    } else {
        printf("0");
    }
}
