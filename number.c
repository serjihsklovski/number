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


static void _number_len_1(String buf, const String source) {
    buf[0] = source[0];     // copy the digit
    buf[1] = '\0';
}


static void _number_len_2(String buf, Bool* negative, Bool* approximate, const String source) {
    switch (source[0]) {
        case '0':   // `source` == "0n" => "n"
        case '+':
            *negative = False;
            *approximate = False;
            buf[0] = source[1];
            buf[1] = '\0';
            break;

        case '-':
            *negative = (source[1] == '0') ? False : True;  // if `source` == "-0" => "0"
            *approximate = False;
            buf[0] = source[1];
            buf[1] = '\0';
            break;

        case '~':
            *negative = False;
            *approximate = True;
            buf[0] = source[1];
            buf[1] = '\0';
            break;

        default:    // `source` == "10".."99"
            *negative = False;
            *approximate = False;
            buf[0] = source[0];
            buf[1] = source[1];
            buf[2] = '\0';
            break;
    }
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

    size_t  len = strlen(number_in_string);
    String  buf = (String) malloc(sizeof(char) * len + 1);
    /*
     * `buf` holds only digit characters: both parts integer and fraction.
     * If `number_in_string` is a sequence of digits ("8763548271"), then
     * `buf` will be completely filled.
     */

    size_t  i = 0;              // `number_in_string` counter
    size_t  j = 0;              // `buf` counter
    char    c;                  // current character in `number_in_string`
    Bool    has_fract = False;  // if the number has fraction part
    size_t  fract_pos = 0;      // position where fraction part of number begins

    /*
     * If `len` == 1, then `number_in_string` holds only one digit (0-9),
     * without any extra characters ('+', '-', '~', and so on). Otherwise,
     * it's not a Number literal at all.
     *
     * If `len` == 2, then `number_in_string` can contain extra characters
     * at the beginning: '+', '-' or '~'. Sequence "-0"must be reduce to the "0".
     *
     * If `len` >= 3, then `number_in_string` can be any of forms:
     * -- integer:         "243"
     * -- with '+':        "+10"
     * -- negative:        "-56"
     * -- real:            "0.1"
     * -- approximate:     "~12"
     * -- scientific:      "2e5"
     * -- with separators: "1_0"
     *                     "1`0"
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
    if (len == 1u) {
        self->_is_negative = False;
        self->_is_approximate = False;
        self->_is_exp_negative = False;
        self->_has_period = False;
        self->_has_endless_fract = False;

        _number_len_1(buf, number_in_string);
    } else if (len == 2u) {
        self->_is_exp_negative = False;
        self->_has_period = False;
        self->_has_endless_fract = False;

        _number_len_2(buf, &self->_is_negative, &self->_is_approximate, number_in_string);
    }

    printf("buf = %s\n", buf);















//    switch (len) {
//        case 2:
//            next_character:

//            c = number_in_string[i];

//            switch (c) {
//                case '~':
//                    self->_is_approximate = 1;
//                    i++;
//                    goto next_character;

//                case '-':
//                    self->_is_negative = 1;

//                case '+':
//                    i++;
//                    break;

//                default:
//                    break;
//            }

//        default:
//            for (; i < len; i++) {
//                c = number_in_string[i];

//                if (isdigit((int) c)) {
//                    buf[j] = c;
//                    j++;
//                } else if (c == '.' && !has_fract) {
//                    has_fract = 1;
//                    fract_pos = j;
//                } else {    // (c == '`') or (c == '_')
//                    j++;
//                }
//            }
//    }

//    buf[j] = '\0';  // j - 1 is len of buf, where j - 1 <= len

//    if (has_fract) {
//        String segment;

//        for (int k = (int) fract_pos - 1; k >= 0; k -= 4) {
//            segment = (String) malloc(sizeof(char) * 5);
//            segment[4] = '\0';

//            if (k / 4 > 0) {
//                segment[3] = buf[k];
//                segment[2] = buf[k - 1];
//                segment[1] = buf[k - 2];
//                segment[0] = buf[k - 3];
//            } else {
//                switch (k % 4) {
//                    case 3:
//                        segment[3] = buf[k];
//                        segment[2] = buf[k - 1];
//                        segment[1] = buf[k - 2];
//                        segment[0] = '0';
//                        break;

//                    case 2:
//                        segment[3] = buf[k];
//                        segment[2] = buf[k - 1];
//                        segment[1] = '0';
//                        segment[0] = '0';
//                        break;

//                    case 1:
//                        segment[3] = buf[k];
//                        segment[2] = '0';
//                        segment[1] = '0';
//                        segment[0] = '0';
//                        break;
//                }
//            }

//            self->_integer->push_front(self->_integer, segment);
//        }

//        for (size_t k = fract_pos; k < j; k += 4) {
//            segment = (String) malloc(sizeof(char) * 5);
//            segment[4] = '\0';

//            if ((j - k) / 4 > 0) {
//                segment[3] = buf[k];
//                segment[2] = buf[k - 1];
//                segment[1] = buf[k - 2];
//                segment[0] = buf[k - 3];
//            } else {
//                switch (k % 4) {
//                    case 3:
//                        segment[3] = '0';
//                        segment[2] = buf[k];
//                        segment[1] = buf[k - 1];
//                        segment[0] = buf[k - 2];
//                        break;

//                    case 2:
//                        segment[3] = '0';
//                        segment[2] = '0';
//                        segment[1] = buf[k];
//                        segment[0] = buf[k - 1];
//                        break;

//                    case 1:
//                        segment[3] = '0';
//                        segment[2] = '0';
//                        segment[1] = '0';
//                        segment[0] = buf[k];
//                        break;
//                }
//            }

//            self->_fraction->push_back(self->_fraction, segment);
//        }
//    }

    free(buf);

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

    if (number->is_negative) {
        printf("-");
    }

    for (size_t i = 0; i < number->_integer->_size; i++) {
        printf("%s", number->_integer->at(number->_integer, i));
    }

    printf(".");

    for (size_t i = 0; i < number->_fraction->_size; i++) {
        printf("%s\n", number->_fraction->at(number->_fraction, i));
    }
}
