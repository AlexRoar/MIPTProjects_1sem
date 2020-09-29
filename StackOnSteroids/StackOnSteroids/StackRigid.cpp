//
//  main.cpp
//  StackOnSteroids
//
//  Created by Александр Дремов on 29.09.2020.
//

#include "StackRigid.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>


typedef int StackElementType;

struct StackRigid{
    uint32_t checkSum;
    size_t capacity;
    size_t size;
    StackElementType data[1];
};


StackRigid* NewStackRigid(const size_t capacity){
    const size_t memory = sizeof(StackRigid) + (capacity - 2) * sizeof(StackElementType);
    StackRigid* pointer = (StackRigid*)calloc(memory, 1);
    if (pointer == NULL){
        return NULL;
    }
    pointer->capacity = capacity;
    pointer->size = 0;
    __StackUpdateChecksum(pointer);
    return pointer;
}

static void __StackUpdateChecksum(StackRigid* stack){
    assert(stack);
    stack->checkSum = __StackGetChecksum(stack);
}

StackRigidState StackValidate(StackRigid* stack){
    if (stack == NULL)
        return STACK_ST_NULL;
    
    if (stack->capacity == 0)
        return STACK_ST_UNINITIALIZED;
    
    if (stack->size > stack->capacity)
        return STACK_ST_INTEGRITYERR;
    
    if (stack->checkSum == 0)
        return STACK_ST_INTEGRITYERR;
    
    uint32_t currentChecksum = __StackGetChecksum(stack);
    if (currentChecksum != stack->checkSum)
        return STACK_ST_INTEGRITYERR;
    
    return STACK_ST_OK;
}

static uint32_t __StackGetChecksum(StackRigid* stack){
    assert(stack);
    void* firstBlock = (void*)((char*)stack + sizeof(stack->capacity));
    const size_t memory = sizeof(StackRigid) + (stack->capacity - 2) * sizeof(StackElementType);
    return adlerChecksum(firstBlock, memory - sizeof(stack->capacity) - sizeof(StackElementType) * (stack->capacity - stack->size));
}

void StackDump(FILE* output, StackRigid* stack){
    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);
    if (output == NULL){
        output = stdout;
        printf("\nWarning! Specified dump output is inavailbale! Selecting stdout.\n");
    }
    fprintf(output, "=================================\n");
    fprintf(output, "Stack dump %s", asctime(ptm));
    if (stack == NULL){
        fprintf(output, "\nStack dump is impossible: NULL pointer\n");
    }else{
        fprintf(output, "Stack (");
        const char *status = "ok";
        switch (StackValidate(stack)) {
            case STACK_ST_INTEGRITYERR:
                status = "INTEGRITY ERROR SPOTTED";
                break;
            case STACK_ST_UNINITIALIZED:
                status = "UNINITIALIZED";
                break;
            case STACK_ST_OK:
                status = "ok";
                break;
            case STACK_ST_NULL:
                status = "NULL POINTER";
                break;
        }
        fprintf(output, "%s", status);
        fprintf(output, ")");
        fprintf(output, " [%p]: {\n", stack);
        
        fprintf(output, "\t    size : %lu\n", stack->size);
        fprintf(output, "\tcapacity : %lu\n", stack->capacity);
        fprintf(output, "\tcheckSum : %x\n", stack->checkSum);
        
        fprintf(output, "\tdata [%p]: {\n", stack->data);
        fprintf(output, "\t}\n");
        fprintf(output, "}\n");
        
        const size_t memory = sizeof(StackRigid) + (stack->capacity - 2) * sizeof(StackElementType);
        fprintf(output, "Size allocated : %lu bytes\n", memory);
        fprintf(output, "Raw Stack size : %lu bytes\n", sizeof(StackRigid));
        fprintf(output, "Element size   : %lu bytes\n", sizeof(StackElementType));
        fprintf(output, "Block end      : %p\n", (char*)stack + memory);
    }
    fprintf(output, "=================================\n");
}


StackRigidOperationCodes StackPush(StackRigid** stack, StackElementType value){
    return STACK_OP_OK;
}


static void __StackRealocate(StackRigid** stack){
    
}


static uint32_t adlerChecksum(const void* firstBlock, size_t len){
    uint32_t a = 1, b = 0;
    const uint32_t MOD_ADLER = 65521;
    
    for (size_t index = 0; index < len; ++index)
    {
        a = (a + ((char*)firstBlock)[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    
    return (b << 16) | a;
}
