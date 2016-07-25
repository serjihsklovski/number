#ifndef TEMPLATE_LIST_H
#define TEMPLATE_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/cclasses/cclasses.h"
#include "lib/cexception/CException.h"
#include "error_codes.h"
#include <stdlib.h>


/* Specific typename */
#define List(T) List_##T


/* This macros generates definition of a specific type of List(T) */
#define TemplateList(T)                                                         \
                                                                                \
typedef struct Node_##T {                                                       \
    T                   _data;                                                  \
    struct Node_##T*    _next;                                                  \
    struct Node_##T*    _prev;                                                  \
} Node_##T;                                                                     \
                                                                                \
                                                                                \
cclass_(List_##T) {                                                             \
    /* fields */                                                                \
    size_t      _size;                                                          \
    Node_##T*   _head;                                                          \
    Node_##T*   _tail;                                                          \
    Node_##T*   _cache;                                                         \
                                                                                \
    /* methods */                                                               \
    method_def_(void,   push_back,  List(T)) with_(T value);                    \
    method_def_(void,   push_front, List(T)) with_(T value);                    \
    method_def_(T,      pop_back,   List(T)) without_args;                      \
    method_def_(T,      pop_front,  List(T)) without_args;                      \
    method_def_(_Bool,  is_empty,   List(T)) without_args;                      \
    method_def_(_Bool,  has_index,  List(T)) with_(int index);                  \
    method_def_(T,      at,         List(T)) with_(int index);                  \
    method_def_(void,   set,        List(T)) with_(int index, T value);         \
    method_def_(void,   insert,     List(T)) with_(int index, T value);         \
    method_def_(void,   append,     List(T)) with_(List(T) lst);                \
    method_def_(void,   prepend,    List(T)) with_(List(T) lst);                \
    method_def_(void,   embed,      List(T)) with_(int index, List(T) lst);     \
    method_def_(T,      pop,        List(T)) with_(int index);                  \
    method_def_(void,   clear,      List(T)) without_args;                      \
    method_def_(void,   swap,       List(T)) with_(int index_1, int index_2);   \
    method_def_(List(T), copy,      List(T)) without_args;                      \
    method_def_(List(T), slice,     List(T)) with_(int beg, int count, int step);   \
    method_def_(void,   reverse,    List(T)) without_args;                      \
    method_def_(List(T), reversed,  List(T)) without_args;                      \
};                                                                              \
                                                                                \
constructor_(List(T))();                                                        \
destructor_(List(T));


#define TemplateListImplementation(T)                                           \
                                                                                \
static int _get_abs_index(int relative_index, size_t size) {                    \
    return relative_index >= 0 ? relative_index : (int) size + relative_index;  \
}                                                                               \
                                                                                \
                                                                                \
static _Bool _near_head(int index, size_t size) {                               \
    int mid = (int) size / 2;                                                   \
    return index <= mid;                                                        \
}                                                                               \
                                                                                \
                                                                                \
static Node_##T* _new_node(T data, Node_##T* next, Node_##T* prev) {            \
    Node_##T* node = (Node_##T*) malloc(sizeof(Node_##T));                      \
                                                                                \
    node->_data = data;                                                         \
    node->_next = next;                                                         \
    node->_prev = prev;                                                         \
                                                                                \
    return node;                                                                \
}                                                                               \
                                                                                \
                                                                                \
static Node_##T* _search_node(List(T) lst, int index) {                         \
    Node_##T* node;                                                             \
    int i;                                                                      \
                                                                                \
    if (_near_head(index, lst->_size)) {                                        \
        node = lst->_head;                                                      \
        i = 0;                                                                  \
                                                                                \
        while (i < index) {                                                     \
            node = node->_next;                                                 \
            ++i;                                                                \
        }                                                                       \
    } else {                                                                    \
        node = lst->_tail;                                                      \
        i = lst->_size - 1;                                                     \
                                                                                \
        while (i > index) {                                                     \
            node = node->_prev;                                                 \
            --i;                                                                \
        }                                                                       \
    }                                                                           \
                                                                                \
    return node;                                                                \
}                                                                               \
                                                                                \
                                                                                \
method_body_(_Bool, is_empty, List(T)) without_args {                           \
    return self->_size == 0;                                                    \
}                                                                               \
                                                                                \
                                                                                \
method_body_(void, push_back, List(T)) with_(T value) {                         \
    Node_##T* new_node = _new_node(value, NULL, NULL);                          \
                                                                                \
    if (self->is_empty(self)) {                                                 \
        self->_head = new_node;                                                 \
        self->_tail = new_node;                                                 \
    } else {                                                                    \
        self->_tail->_next = new_node;                                          \
        new_node->_prev = self->_tail;                                          \
        self->_tail = new_node;                                                 \
    }                                                                           \
                                                                                \
    ++self->_size;                                                              \
}                                                                               \
                                                                                \
                                                                                \
method_body_(void, push_front, List(T)) with_(T value) {                        \
    Node_##T* new_node = _new_node(value, NULL, NULL);                          \
                                                                                \
    if (self->is_empty(self)) {                                                 \
        self->_head = new_node;                                                 \
        self->_tail = new_node;                                                 \
    } else {                                                                    \
        self->_head->_prev = new_node;                                          \
        new_node->_next = self->_head;                                          \
        self->_head = new_node;                                                 \
    }                                                                           \
                                                                                \
    ++self->_size;                                                              \
}                                                                               \
                                                                                \
                                                                                \
method_body_(T, pop_back, List(T)) without_args {                               \
    if (self->is_empty(self)) {                                                 \
        Throw(EMPTY_LIST);                                                      \
    }                                                                           \
                                                                                \
    T value = self->_tail->_data;                                               \
                                                                                \
    if (self->_size > 1) {                                                      \
        self->_tail->_prev->_next = NULL;                                       \
        Node_##T* temp = self->_tail;                                           \
        self->_tail = temp->_prev;                                              \
        free(temp);                                                             \
    } else {    /* 1 element in the list */                                     \
        self->_head = NULL;                                                     \
        free(self->_tail);                                                      \
        self->_tail = NULL;                                                     \
    }                                                                           \
                                                                                \
    --self->_size;                                                              \
                                                                                \
    return value;                                                               \
} throws_(EMPTY_LIST)                                                           \
                                                                                \
                                                                                \
method_body_(T, pop_front, List(T)) without_args {                              \
    if (self->is_empty(self)) {                                                 \
        Throw(EMPTY_LIST);                                                      \
    }                                                                           \
                                                                                \
    T value = self->_head->_data;                                               \
                                                                                \
    if (self->_size > 1) {                                                      \
        self->_head->_next->_prev = NULL;                                       \
        Node_##T* temp = self->_head;                                           \
        self->_head = temp->_next;                                              \
        free(temp);                                                             \
    } else {    /* 1 element in the list */                                     \
        self->_tail = NULL;                                                     \
        free(self->_head);                                                      \
        self->_head = NULL;                                                     \
    }                                                                           \
                                                                                \
    --self->_size;                                                              \
                                                                                \
    return value;                                                               \
} throws_(EMPTY_LIST)                                                           \
                                                                                \
                                                                                \
method_body_(_Bool, has_index, List(T)) with_(int index) {                      \
    index = _get_abs_index(index, self->_size);                                 \
                                                                                \
    if (index >= 0 && index < (int) self->_size) {                              \
        return 1;                                                               \
    }                                                                           \
                                                                                \
    return 0;                                                                   \
}                                                                               \
                                                                                \
                                                                                \
method_body_(void, clear, List(T)) without_args {                               \
    while (!self->is_empty(self)) {                                             \
        if (self->_size > 1) {                                                  \
            self->_head->_next->_prev = NULL;                                   \
            Node_##T* temp = self->_head;                                       \
            self->_head = temp->_next;                                          \
            free(temp);                                                         \
            temp->_next = NULL;                                                 \
            temp->_prev = NULL;                                                 \
        } else {    /* only 1 element in the list */                            \
            free(self->_head);                                                  \
            self->_head = NULL;                                                 \
            self->_tail = NULL;                                                 \
        }                                                                       \
                                                                                \
        --self->_size;                                                          \
    }                                                                           \
}                                                                               \
                                                                                \
                                                                                \
method_body_(T, at, List(T)) with_(int index) {                                 \
    if (!self->has_index(self, index)) {                                        \
        Throw(INDEX_IS_OUT_OF_RANGE);                                           \
    }                                                                           \
                                                                                \
    return _search_node(self, _get_abs_index(index, self->_size))->_data;       \
} throws_(INDEX_IS_OUT_OF_RANGE)                                                \
                                                                                \
                                                                                \
method_body_(void, set, List(T)) with_(int index, T value) {                    \
    if (!self->has_index(self, index)) {                                        \
        Throw(INDEX_IS_OUT_OF_RANGE);                                           \
    }                                                                           \
                                                                                \
    _search_node(self, _get_abs_index(index, self->_size))->_data = value;      \
} throws_(INDEX_IS_OUT_OF_RANGE)                                                \
                                                                                \
                                                                                \
method_body_(void, insert, List(T)) with_(int index, T value) {                 \
    if (!self->has_index(self, index)) {                                        \
        Throw(INDEX_IS_OUT_OF_RANGE);                                           \
    }                                                                           \
                                                                                \
    index = _get_abs_index(index, self->_size);                                 \
                                                                                \
    if (index == 0) {                                                           \
        self->push_front(self, value);                                          \
    } else {                                                                    \
        Node_##T* node = _search_node(self, index);                             \
        Node_##T* new_node = _new_node(value, node, node->_prev);               \
                                                                                \
        node->_prev = new_node;                                                 \
        new_node->_prev->_next = new_node;                                      \
                                                                                \
        ++self->_size;                                                          \
    }                                                                           \
} throws_(INDEX_IS_OUT_OF_RANGE)                                                \
                                                                                \
                                                                                \
method_body_(void, append, List(T)) with_(List(T) lst) {                        \
    if (!lst->is_empty(lst)) {                                                  \
        Node_##T* node = lst->_head;                                            \
        Node_##T* new_node;                                                     \
                                                                                \
        if (self->is_empty(self)) {                                             \
            self->_head = self->_tail = _new_node(node->_data, NULL, NULL);     \
            node = node->_next;                                                 \
        }                                                                       \
                                                                                \
        while (node != NULL) {                                                  \
            new_node = _new_node(node->_data, NULL, self->_tail);               \
            self->_tail->_next = new_node;                                      \
            self->_tail = new_node;                                             \
            node = node->_next;                                                 \
        }                                                                       \
                                                                                \
        self->_size += lst->_size;                                              \
    }                                                                           \
}                                                                               \
                                                                                \
                                                                                \
method_body_(void, prepend, List(T)) with_(List(T) lst) {                       \
    if (!lst->is_empty(lst)) {                                                  \
        Node_##T* node = lst->_tail;                                            \
        Node_##T* new_node;                                                     \
                                                                                \
        if (self->is_empty(self)) {                                             \
            self->_head = self->_tail = _new_node(node->_data, NULL, NULL);     \
            node = node->_prev;                                                 \
        }                                                                       \
                                                                                \
        while (node != NULL) {                                                  \
            new_node = _new_node(node->_data, self->_head, NULL);               \
            self->_head->_prev = new_node;                                      \
            self->_head = new_node;                                             \
            node = node->_prev;                                                 \
        }                                                                       \
                                                                                \
        self->_size += lst->_size;                                              \
    }                                                                           \
}                                                                               \
                                                                                \
                                                                                \
method_body_(void, embed, List(T)) with_(int index, List(T) lst) {              \
    if (!self->has_index(self, index)) {                                        \
        Throw(INDEX_IS_OUT_OF_RANGE);                                           \
    }                                                                           \
                                                                                \
    index = _get_abs_index(index, self->_size);                                 \
                                                                                \
    if (index == 0) {                                                           \
        self->prepend(self, lst);                                               \
    } else if (!lst->is_empty(lst)) {                                           \
        Node_##T* head = _search_node(self, index);                             \
        Node_##T* node = lst->_head;                                            \
        Node_##T* new_node;                                                     \
                                                                                \
        while (node != NULL) {                                                  \
            new_node = _new_node(node->_data, head, head->_prev);               \
            head->_prev = new_node;                                             \
            new_node->_prev->_next = new_node;                                  \
            node = node->_next;                                                 \
        }                                                                       \
                                                                                \
        self->_size += lst->_size;                                              \
    }                                                                           \
} throws_(INDEX_IS_OUT_OF_RANGE)                                                \
                                                                                \
                                                                                \
method_body_(T, pop, List(T)) with_(int index) {                                \
    if (!self->has_index(self, index)) {                                        \
        Throw(INDEX_IS_OUT_OF_RANGE);                                           \
    }                                                                           \
                                                                                \
    index = _get_abs_index(index, self->_size);                                 \
                                                                                \
    if (index == 0) {   /* first element */                                     \
        return self->pop_front(self);                                           \
    } else if (index == (int) self->_size - 1) {    /* last element */          \
        return self->pop_back(self);                                            \
    } else {    /* element in the middle */                                     \
        Node_##T*   node = _search_node(self, index);                           \
        T           value = node->_data;                                        \
                                                                                \
        node->_next->_prev = node->_prev;                                       \
        node->_prev->_next = node->_next;                                       \
                                                                                \
        node->_next = NULL;                                                     \
        node->_prev = NULL;                                                     \
                                                                                \
        free(node);                                                             \
        node = NULL;                                                            \
        --self->_size;                                                          \
                                                                                \
        return value;                                                           \
    }                                                                           \
} throws_(INDEX_IS_OUT_OF_RANGE)                                                \
                                                                                \
                                                                                \
method_body_(void, swap, List(T)) with_(int index_1, int index_2) {             \
    if (!self->has_index(self, index_1) ||                                      \
        !self->has_index(self, index_2))                                        \
    {                                                                           \
        Throw(INDEX_IS_OUT_OF_RANGE);                                           \
    }                                                                           \
                                                                                \
                                                                                \
    index_1 = _get_abs_index(index_1, self->_size);                             \
    index_2 = _get_abs_index(index_2, self->_size);                             \
                                                                                \
    if (index_1 != index_2) {                                                   \
        Node_##T* node_1 = _search_node(self, index_1);                         \
        Node_##T* node_2 = _search_node(self, index_2);                         \
        T value = node_1->_data;                                                \
                                                                                \
        node_1->_data = node_2->_data;                                          \
        node_2->_data = value;                                                  \
    }                                                                           \
} throws_(INDEX_IS_OUT_OF_RANGE)                                                \
                                                                                \
                                                                                \
method_body_(List(T), copy, List(T)) without_args {                             \
    List(T) new_lst = new_(List_##T)();                                         \
                                                                                \
    for (Node_##T* it = self->_head; it != NULL; it = it->_next) {              \
        new_lst->push_back(new_lst, it->_data);                                 \
    }                                                                           \
                                                                                \
    return new_lst;                                                             \
}                                                                               \
                                                                                \
                                                                                \
method_body_(List(T), slice, List(T)) with_(int beg, int count, int step) {     \
    if (!self->has_index(self, beg)) {                                          \
        Throw(INDEX_IS_OUT_OF_RANGE);                                           \
    }                                                                           \
                                                                                \
    if (count < 0) {                                                            \
        Throw(NEGATIVE_COUNT);                                                  \
    }                                                                           \
                                                                                \
    beg = _get_abs_index(beg, self->_size);                                     \
                                                                                \
    if (step == 0) {                                                            \
        Throw(ZERO_STEP);                                                       \
    } else if (step > 0) {                                                      \
        if (beg + count > (int) self->_size) {                                  \
            Throw(INDEX_IS_OUT_OF_RANGE);                                       \
        }                                                                       \
    } else {    /* step < 0 */                                                  \
        if (beg - count < -1) {                                                 \
            Throw(INDEX_IS_OUT_OF_RANGE);                                       \
        }                                                                       \
    }                                                                           \
                                                                                \
    Node_##T*   it = _search_node(self, beg);                                   \
    List(T)     new_lst = new_(List_##T)();                                     \
                                                                                \
    if (step > 0) {                                                             \
        for (int i = 0; i < count; ++i) {                                       \
            if (i % step == 0) {                                                \
                new_lst->push_back(new_lst, it->_data);                         \
            }                                                                   \
                                                                                \
            it = it->_next;                                                     \
        }                                                                       \
    } else {    /* step < 0 */                                                  \
        step = -step;                                                           \
                                                                                \
        for (int i = 0; i < count; ++i) {                                       \
            if (i % step == 0) {                                                \
                new_lst->push_back(new_lst, it->_data);                         \
            }                                                                   \
                                                                                \
            it = it->_prev;                                                     \
        }                                                                       \
    }                                                                           \
                                                                                \
    return new_lst;                                                             \
} throws_(INDEX_IS_OUT_OF_RANGE, NEGATIVE_COUNT, ZERO_STEP)                     \
                                                                                \
                                                                                \
method_body_(void, reverse, List(T)) without_args {                             \
    for (int i = 0; i < (int) self->_size / 2; ++i) {                           \
        self->swap(self, i, -(i + 1));                                          \
    }                                                                           \
}                                                                               \
                                                                                \
                                                                                \
method_body_(List(T), reversed, List(T)) without_args {                         \
    List(T) new_lst = new_(List_##T)();                                         \
                                                                                \
    for (Node_##T* node = self->_head; node != NULL; node = node->_next) {      \
        new_lst->push_front(new_lst, node->_data);                              \
    }                                                                           \
                                                                                \
    return new_lst;                                                             \
}                                                                               \
                                                                                \
                                                                                \
constructor_(List(T))() {                                                       \
    new_self_(List_##T);                                                        \
                                                                                \
    self->_size = 0;                                                            \
    self->_head = NULL;                                                         \
    self->_tail = NULL;                                                         \
    self->_cache = NULL;                                                        \
                                                                                \
    init_method_(is_empty);                                                     \
    init_method_(push_back);                                                    \
    init_method_(push_front);                                                   \
    init_method_(pop_back);                                                     \
    init_method_(pop_front);                                                    \
    init_method_(has_index);                                                    \
    init_method_(clear);                                                        \
    init_method_(at);                                                           \
    init_method_(set);                                                          \
    init_method_(insert);                                                       \
    init_method_(append);                                                       \
    init_method_(prepend);                                                      \
    init_method_(embed);                                                        \
    init_method_(pop);                                                          \
    init_method_(swap);                                                         \
    init_method_(copy);                                                         \
    init_method_(slice);                                                        \
    init_method_(reverse);                                                      \
    init_method_(reversed);                                                     \
                                                                                \
    return self;                                                                \
}                                                                               \
                                                                                \
                                                                                \
destructor_(List(T)) {                                                          \
    self->clear(self);                                                          \
    free(self);                                                                 \
    self = NULL;                                                                \
}


#ifdef __cplusplus
}
#endif

#endif // TEMPLATE_LIST_H
