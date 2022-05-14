#ifndef UTILITIES_H
#define UTILITIES_H 
#include <stdint.h>
/*
https://en.cppreference.com/w/c/language/operator_other#Conditional_operator
3) Performs a conversion from the result of the evaluation to the common type, defined as follows:
1) if the expressions have arithmetic type, the common type is the type after usual arithmetic conversions
2) if the expressions have struct/union type, the common type is that struct/union type
3) if the expressions are both void, the entire conditional operator expression is a void expression
4) if one is a pointer and the other is a null pointer constant, the type is the type of that pointer
5) if both are pointers, the result is the pointer to the type that combines cvr-qualifiers of both pointed-to types 
    (that is, if one is const int* and the other is volatile int*, 
    the result is const volatile int*), 
    and if the types were different, the pointed-to type is the composite type.
6) if one is a pointer to void, the result is a pointer to void with combined cvr-qualifiers
*/
#define is_immediate(x) ( \
    _Generic( (0 ?  (void*)(uintptr_t)((x) - (x)) : (int*)(0x40000000ULL)), \
        int*: 1, \
        void*: 0, \
        default: 0 \
    ) \
)


#define ROUND_DOWN(number, target) (((number) / (target)) * (target)) 
#define ROUND_UP(number, target) ((((number) + (target) - 1) / (target)) * (target)) 
#endif 



