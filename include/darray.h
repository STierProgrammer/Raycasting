// NOTE: Inspired by nob's da_append macros
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define da_reserve_exact(da, exact) \
    do {\
       if((exact) > (da)->cap) {\
           (da)->cap = (exact);\
           (da)->items = realloc((da)->items, (da)->cap*sizeof(*(da)->items));\
           assert((da)->items && "Ran out of memory");\
       }\
    } while(0)
#define da_reserve(da, extra) \
    do {\
       if(extra && (da)->len + extra >= (da)->cap) {\
           (da)->cap = (da)->cap*2+extra;\
           (da)->items = realloc((da)->items, (da)->cap*sizeof(*(da)->items));\
           assert((da)->items && "Ran out of memory");\
       }\
    } while(0)
#define da_push(da, value) \
    do {\
        da_reserve(da, 1);\
        (da)->items[(da)->len++]=value;\
    } while(0)
#define da_append_many(da, values, count) \
    do { \
        da_reserve(da, count); \
        memcpy((da)->items + (da)->len, (values), (count) * sizeof(*(da)->items)); \
        (da)->len += (count); \
    } while(0)
#define da_insert(da, index, value) \
    do {\
         assert((int)(index) <= (int)(da)->len && "Index out of bounds");\
         da_reserve(da, 1);\
         memmove(&(da)->items[(index) + 1], &(da)->items[(index)], ((da)->len - (index)) * sizeof(*(da)->items));\
         (da)->items[(index)] = (value);\
         (da)->len++;\
    } while(0)

#define da_remove_unordered(da, index) (assert((index) < (da)->len), (da)->items[(index)] = (da)->items[--(da)->len])
#define da_resize_exact(da, count) \
    do { \
        da_reserve_exact(da, (count)); \
        (da)->len = (count); \
    } while(0)
#define da_free(da) free((da).items)

