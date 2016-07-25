#ifndef CCLASSES_H
#define CCLASSES_H

#ifdef __cplusplus
extern "C" {
#endif


/* Class creating */
#define cclass_(ClassName)          struct _##ClassName##_t;\
                                    typedef struct _##ClassName##_t* ClassName;\
                                    struct _##ClassName##_t

#define extends_(ClassName)         struct _##ClassName##_t


/* Methods */
#define method_def_(retType, methodName, ClassName)\
    retType (*methodName)(ClassName self
#define method_body_(retType, methodName, ClassName)\
    static retType _##methodName(ClassName self
#define throws_(...);


/* Arguments */
#define with_(...)                  , __VA_ARGS__)
#define without_args                )


/* Object creating */
#define new_(ClassName)             new_##ClassName     // constructor name
#define delete_(ClassName)          delete_##ClassName  // destructor name
#define constructor_(ClassName)     ClassName new_(ClassName)
#define destructor_(ClassName)      void delete_(ClassName)(ClassName self)
#define new_self_(ClassName)        ClassName self = (ClassName) malloc(sizeof (struct _##ClassName##_t))
#define init_method_(methodName)    self->methodName = _##methodName


#ifdef __cplusplus
}
#endif

#endif // CCLASSES_H
