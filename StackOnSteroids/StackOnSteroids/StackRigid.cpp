/**
 * @file
 */

#include "StackRigid.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

/**
 * If defined, collects average speed information during the run
 */
#define DEBUGSPEED 1

#ifdef DEBUGSPEED
    // 0 - average checksum update time
    // 1 - average validation time
    // 2 - average push time
    // 3 - average pop time
    // 4 - average realocate time
    const double movingAverageBetta = 0.85;
    double StackRigidAvgTimes[5] = {};
#endif


struct StackRigid {
    uint32_t checkSum;
    uint32_t checkSumVital;
    size_t capacity;
    size_t size;
    FILE* logFile;
    StackElementType data[1];
};


StackRigid* NewStackRigid(const size_t capacity, FILE* logFile) {
    const size_t memory = sizeof(StackRigid) + (capacity - 2) * sizeof(StackElementType);
    StackRigid* pointer = (StackRigid*)calloc(memory, 1);
    if (pointer == NULL) {
        return NULL;
    }
    
    pointer->capacity = capacity;
    pointer->size = 0;
    pointer->logFile = logFile;
    pointer->checkSum = 0;
    pointer->checkSumVital = 0;
    __StackUpdateChecksum(pointer);
    return pointer;
}


StackRigidOperationCodes StackPush(StackRigid** stack, StackElementType value) {
    #ifdef DEBUGSPEED
        clock_t start = clock();
    #endif
    if (stack == NULL)
        return STACK_OP_NULL;
    
    StackRigidState integrityChecks = StackValidate(*stack);
    if (integrityChecks != STACK_ST_OK) {
        StackDump(*stack);
        return STACK_OP_INTEGRITYERR;
    }
    
    StackRigidOperationCodes realocResult = __StackRealocate(stack, 1);
    if (realocResult != STACK_OP_OK) {
        StackDump(*stack);
        return realocResult;
    }
    
    (*stack)->data[(*stack)->size] = value;
    (*stack)->size += 1;
    
    __StackUpdateChecksum(*stack);
    
    if ((*stack)->size == 0) {
        return STACK_OP_OVERFLOW;
    }
    #ifdef DEBUGSPEED
        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        StackRigidAvgTimes[2] = StackRigidAvgTimes[2] * (movingAverageBetta) + elapsed * (1 - movingAverageBetta);
    #endif
    return STACK_OP_OK;
}


StackRigidOperationCodes StackPop(StackRigid** stack, StackElementType* value) {
    #ifdef DEBUGSPEED
        clock_t start = clock();
    #endif
    
    if (stack == NULL)
        return STACK_OP_NULL;
    
    StackRigidState integrityChecks = StackValidate(*stack);
    if (integrityChecks != STACK_ST_OK) {
        StackDump(*stack);
        return STACK_OP_INTEGRITYERR;
    }
    
    if((*stack)->size == 0)
        return STACK_OP_UNDERFLOW;
    
    *value = (*stack)->data[(*stack)->size - 1];
    (*stack)->size -= 1;
    
    __StackRealocate(stack, -1);
    __StackUpdateChecksum(*stack);
    
    #ifdef DEBUGSPEED
        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        StackRigidAvgTimes[3] = StackRigidAvgTimes[3] * (movingAverageBetta) + elapsed * (1 - movingAverageBetta);
    #endif
    
    return STACK_OP_OK;
}


StackRigidOperationCodes StackBack(StackRigid* stack, StackElementType* value){
    if (stack == NULL)
        return STACK_OP_NULL;
    
    StackRigidState integrityChecks = StackValidate(stack);
    if (integrityChecks != STACK_ST_OK) {
        StackDump(stack);
        return STACK_OP_INTEGRITYERR;
    }
    
    if(stack->size == 0)
        return STACK_OP_UNDERFLOW;
    
    *value = stack->data[stack->size - 1];
    return STACK_OP_OK;
}


StackRigidState StackValidate(StackRigid* stack) {
    #ifdef DEBUGSPEED
        clock_t start = clock();
    #endif
    
    if (stack == NULL)
        return STACK_ST_NULL;
    
    if (stack->size > stack->capacity || stack->checkSum == 0 || stack->checkSumVital == 0)
        return STACK_ST_INTEGRITYERR;
    
    uint32_t currentChecksumVital = __StackGetChecksumVital(stack);
    if (currentChecksumVital != stack->checkSumVital || currentChecksumVital == 0)
        return STACK_ST_INTEGRITYERR;
    
    uint32_t currentChecksum = __StackGetChecksum(stack);
    if (currentChecksum != stack->checkSum || currentChecksum == 0)
        return STACK_ST_INTEGRITYERR;
    
    #ifdef DEBUGSPEED
        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        StackRigidAvgTimes[1] = StackRigidAvgTimes[1] * (movingAverageBetta) + elapsed * (1 - movingAverageBetta);
    #endif
    return STACK_ST_OK;
}


static void __StackUpdateChecksum(StackRigid* stack) {
    assert(stack);
    
    #ifdef DEBUGSPEED
        clock_t start = clock();
    #endif
    
    stack->checkSumVital = __StackGetChecksumVital(stack);
    stack->checkSum = __StackGetChecksum(stack);
    
    #ifdef DEBUGSPEED
        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    
        StackRigidAvgTimes[0] = StackRigidAvgTimes[0] * (movingAverageBetta) + elapsed * (1 - movingAverageBetta);
    #endif
}


static uint32_t __StackGetChecksumVital(StackRigid* stack) {
    assert(stack);
    
    void* firstBlock = (char*)stack + sizeof(stack->checkSum) + sizeof(stack->checkSumVital);
    
    const size_t memory = sizeof(stack->size) + sizeof(stack->capacity)+ sizeof(stack->logFile);
    if (stack->size > stack->capacity) {
        return 0;
    }
    return adlerChecksum(firstBlock, memory);
}


static uint32_t __StackGetChecksum(StackRigid* stack) {
    assert(stack);
    
    void* firstBlock = (char*)stack + sizeof(stack->checkSum);
    
    // Memory used by Stack excluding checkSum and free space
    const size_t memory = StackRigidMemoryUse(stack) - sizeof(stack->checkSum) - (stack->capacity - stack->size) * sizeof(StackElementType);
    
    if (stack->size > stack->capacity) {
        return 0;
    }
    return adlerChecksum(firstBlock, memory);
}


static StackRigidOperationCodes __StackRealocate(StackRigid** stack, short int direction) {
    #ifdef DEBUGSPEED
        clock_t start = clock();
    #endif
    if ((*stack)->capacity == 0) {
        (*stack)->capacity = 16;
        
        const size_t memory = StackRigidMemoryUse(*stack);
        
        StackRigid* newStack = (StackRigid*) realloc((*stack), memory);
        if (newStack == NULL)
            return STACK_OP_NOMEMORY;
        
        (*stack) = newStack;
    }else if(((*stack)->capacity <= (*stack)->size) && direction > 0) {
        // Up realocation
        size_t newCapacity = (*stack)->size * 2;
        
        if (newCapacity <= (*stack)->size) { //   If we exceeded size_t range
            newCapacity = (*stack)->size + 16; // Try to add 16 free spaces
            if (newCapacity < (*stack)->size) //  Even if in this case we still exceed size_t
                return STACK_OP_OVERFLOW;
        }
        
        const size_t memoryNow = StackRigidMemoryUse(*stack);
        const size_t memoryNew = sizeof(StackRigid) + (newCapacity - 2) * sizeof(StackElementType);
        
        if (memoryNew >= memoryNow) {
            StackRigid* newStack = (StackRigid*) realloc((*stack), memoryNew);
            if (newStack == NULL)
                return STACK_OP_NOMEMORY;
            
            (*stack) = newStack;
        }else{
            return STACK_OP_OVERFLOW; // Exceded size_t memory
        }
        (*stack)->capacity = newCapacity;
        
    }else if (((*stack)->capacity / 2.2 > (*stack)->size) && direction < 0) {
        // Down realocation
        size_t newCapacity = (*stack)->capacity / 2.2;
        
        const size_t memoryNow = StackRigidMemoryUse(*stack);
        const size_t memoryNew = sizeof(StackRigid) + (newCapacity - 2) * sizeof(StackElementType);
        
        if (memoryNew <= memoryNow) {
            StackRigid* newStack = (StackRigid*) realloc((*stack), memoryNew);
            if (newStack == NULL)
                return STACK_OP_NOMEMORY;
            
            (*stack) = newStack;
            (*stack)->capacity = newCapacity;
        }else
            return STACK_OP_OVERFLOW; // Exceded size_t memory
    }
    #ifdef DEBUGSPEED
        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        StackRigidAvgTimes[4] = StackRigidAvgTimes[4] * (movingAverageBetta) + elapsed * (1 - movingAverageBetta);
    #endif
    return STACK_OP_OK;
}


static uint32_t adlerChecksum(const void* firstBlock, size_t len) {
    uint32_t a = 1, b = 0;
    const uint32_t MOD_ADLER = 65521;
    for (size_t index = 0; index < len; ++index) {
        a = (a + ((char*)firstBlock)[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    return (b << 16) | a;
}


size_t StackRigidMemoryUse(StackRigid* stack) {
    assert(stack);
    return sizeof(StackRigid) + (stack->capacity - 2) * sizeof(StackElementType);
}


void StackDump(StackRigid* stack) {
    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);
    
    FILE* output = stack->logFile;
    if (output == NULL) {
        output = stdout;
        printf("\nWarning! Specified dump output is inavailbale! Selecting stdout.\n");
    }
    
    fprintf(output, "=================================\n");
    fprintf(output, "Stack dump %s", asctime(ptm));
    
    if (stack == NULL) {
        fprintf(output, "\nStack dump is impossible: NULL pointer\n");
    }else{
        fprintf(output, "Stack (");
        const char *status = "ok";
        StackRigidState checks = StackValidate(stack);
        switch (checks) {
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
        if (checks == STACK_ST_INTEGRITYERR) {
            fprintf(output, "ERROR! Stack structure was corrupted. The data below was changed from the outside.\n");
        }
        fprintf(output, "\t    size : %lu\n", stack->size);
        fprintf(output, "\tcapacity : %lu\n", stack->capacity);
        fprintf(output, "\tcheckSum : %x\n", stack->checkSum);
        
        fprintf(output, "\tdata [%p]: {\n", stack->data);
        size_t i = 0;
        for (i = 0; i < stack->size; i++) {
            fprintf(output, "\t\t");
            fprintf(output, "*[%lu] : %d\n", i, stack->data[i]);
        }
        fprintf(output, "\t\t _______\n");
        if (stack->size < stack->capacity) {
            fprintf(output, "\t\t [%lu] : GARBAGE(%d)\n", i + 1, stack->data[i + 1]);
        }
        if (stack->size + 1 < stack->capacity && stack->size + 1 > stack->size) {
            fprintf(output, "\t\t [%lu] : GARBAGE(%d)\n", i + 2, stack->data[i + 2]);
        }
        fprintf(output, "\t}\n");
        fprintf(output, "}\n");
        
        const size_t memory = StackRigidMemoryUse(stack);
        fprintf(output, "Size allocated : %lu bytes\n", memory);
        fprintf(output, "Raw Stack size : %lu bytes\n", sizeof(StackRigid));
        fprintf(output, "  Element size : %lu bytes\n", sizeof(StackElementType));
        fprintf(output, "     Block end : %p\n", (char*)stack + memory);
    }
    fprintf(output, "=================================\n");
}

StackRigidOperationCodes StackDestruct(StackRigid** stack) {
    if (stack == NULL) {
        return STACK_OP_NULL;
    }
    if (*stack == NULL) {
        return STACK_OP_NULL;
    }
    free(*stack);
    return STACK_OP_OK;
}

#undef DEBUGSPEED
