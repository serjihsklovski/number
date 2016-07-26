#include <number.h>
#include <string.h>
#include <stdio.h>


static _Bool _add_four_digits(String res, const String op_1, const String op_2) {
    // annotation: memory for res has to be allocated!
    const int   MAX_COUNT = 6;
    char        buf[MAX_COUNT];
    _Bool       overflow_flg = 0;

    itoa(atoi(op_1) + atoi(op_2), buf, 10);     // 10 - decimal radix

    /*
     * minimum: 0 + 0 == 0           -> buf == {'0', '\0'}                     -> 2 characters
     * maximum: 9999 + 9999 == 19998 -> buf == {'1', '9', '9', '9', '8', '\0'} -> 6 characters
     */

    size_t len = strnlen(buf, MAX_COUNT);

    if (len < 5) {
        for (unsigned int i = 0; i < 4 - len; i++) {
            strncat(res, "0", MAX_COUNT);
        }

        strncat(res, buf, MAX_COUNT);
    } else {
        overflow_flg = 1;
        strncpy(res, buf + 1, MAX_COUNT);
    }

    return overflow_flg;
}
