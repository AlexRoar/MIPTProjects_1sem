#if 0
Вы должны реализовать структуру данных stack.

Необходимо обеспечить следующие абстракции:

stack * stack_new(size_t size);
stack_code stack_delete(stack *st)
stack_code stack_push(stack *st, int value);
stack_code stack_pop(stack *st, int *value);
Здесь typedef enum stack_code_e { STACK_OK, STACK_NULL, STACK_OVERFLOW, STACK_UNDERFLOW } stack_code;

Этот перечислимый тип уже задан и вам его переопределять нельзя.
Вы должны написать структуру, которая будет использоваться в написанных вами четырёх функциях.

Должно быть предусмотрено, что может создаваться и уничтожаться произвольное количество стеков. Все неверные операции должны возвращать соответствующие коды ошибок.

Функцию main реализовать не нужно. Посланный вами файл должен содержать лишь определение вашей структуры и эти четыре функции.
#endif

#include <stdlib.h>
#include <stdio.h>

//typedef enum stack_code_e { STACK_OK, STACK_NULL, STACK_OVERFLOW, STACK_UNDERFLOW } stack_code;

typedef struct stack {
    int* values;
    size_t sizeMax;
    size_t currentSize;
    stack_code (*stack_delete)(struct stack *st);
    stack_code (*stack_push)(struct stack *st, int value);
    stack_code (*stack_pop)(struct stack *st, int *value);
} stack;

stack_code stack_delete(stack *st){
    if (st == NULL)
        return STACK_NULL;
    if (st->values == NULL)
        return STACK_NULL;
    
    free(st->values);
    free(st);
    return STACK_OK;
}

stack_code stack_push(stack *st, int value){
    if (st == NULL)
        return STACK_NULL;
    if (st->values == NULL)
        return STACK_NULL;
    
    if(st->sizeMax <= st->currentSize)
        return STACK_OVERFLOW;
    
    st->values[st->currentSize] = value;
    st->currentSize += 1;
    return STACK_OK;
}

stack_code stack_pop(stack *st, int *value){
    if (st == NULL)
        return STACK_NULL;
    if (st->values == NULL)
        return STACK_NULL;
    
    if(st->currentSize == 0)
        return STACK_UNDERFLOW;
    st->currentSize -= 1;
    *value = st->values[st->currentSize];
    return STACK_OK;
}

stack* stack_new(size_t size){
    stack* new = calloc(1, sizeof(stack));
    new->values = calloc(size, sizeof(int));
    new->sizeMax = size;
    new->currentSize = 0;
    new->stack_delete = &stack_delete;
    new->stack_pop = &stack_pop;
    new->stack_push = &stack_push;
    return new;
}

//int main(){
//    stack *a = stack_new(10);
//    a->stack_push(a, 4);
//    a->stack_push(a, 10);
//    int b = 0;
//    stack_code s = stack_pop(a, &b);
//    printf("%d %d\n", b, s);
//}
