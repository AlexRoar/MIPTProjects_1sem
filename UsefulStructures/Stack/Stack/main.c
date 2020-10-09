#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int StackElem;

typedef struct{
    StackElem* storage;
    size_t capacity;
    size_t size;
} Stack;


Stack* NewStack(size_t maxSize){
    Stack* newStack = calloc(1, sizeof(Stack));
    newStack->size = 0;
    newStack->capacity = maxSize;
    newStack->storage = calloc(maxSize, sizeof(StackElem));
    if (newStack->storage == NULL){
        free(newStack);
        return NULL;
    }
    return newStack;
}

void StackResize(Stack* stack){
    assert(stack != NULL);
    if (stack->size == stack->capacity){
        stack->storage = realloc(stack->storage, stack->capacity * 2 * sizeof(StackElem));
        stack->capacity *= 2;
    }
}


void StackPush(Stack* stack, StackElem elem){
    assert(stack != NULL);
    StackResize(stack);
    if (stack->size < stack->capacity){
        stack->storage[(stack->size)++] = elem;
    }else{
        return;
    }
}


void StackPop(Stack* stack, StackElem* elem){
    assert(stack != NULL);
    if (stack->size > 0){
        if (elem != NULL){
            *elem = stack->storage[--(stack->size)];
        } else{
            (stack->size)--;
        }
    }else{
        return;
    }
}


void StackTop(Stack* stack, StackElem* elem){
    assert(stack != NULL);
    assert(elem != NULL);
    if (stack->size > 0){
        *elem = stack->storage[(stack->size) - 1];
    }else{
        return;
    }
}


void DestructStack(Stack* stack){
    free(stack->storage);
    free(stack);
}


void StackPrint(Stack* stack){
    for(size_t i = 0; i < stack->size; i++){
        printf("%d ", stack->storage[i]);
    }
    printf("\n");
}


int main(){
    Stack* newStack = NewStack(1);
    
    StackPush(newStack, 1);
    StackPush(newStack, 2);
    StackPush(newStack, 3);
    StackPush(newStack, 4);
    StackPrint(newStack);
    StackPop(newStack, NULL);
    StackPrint(newStack);
    StackPop(newStack, NULL);
    StackPop(newStack, NULL);
    StackPop(newStack, NULL);
    StackPrint(newStack);
    
    DestructStack(newStack);
}
