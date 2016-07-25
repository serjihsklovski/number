#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#ifdef __cplusplus
extern "C" {
#endif

enum ErrorCodes {
    EMPTY_LIST,
    INDEX_IS_OUT_OF_RANGE,
    NEGATIVE_COUNT,
    ZERO_STEP
};

#ifdef __cplusplus
}
#endif

#endif // ERROR_CODES_H
