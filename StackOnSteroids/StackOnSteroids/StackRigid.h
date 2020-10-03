/**
 * @mainpage
 * @author Aleksandr Dremov
 * @brief This Stack is almost impossible to kill and fool. All undefined behaviour memory accesses will be spoted and reported.
 * @attention Before including .h file, you need to define  StackElementDump(FILE, VALUE) and StackElementType \n
 * Example:\n
 * #define StackElementDump(FILE, VALUE) {fprintf(FILE, "%g", VALUE);}\n
 * #define StackElementType double\n
 * @copyright Aleksandr Dremov, MIPT 2020
 */


#undef __overload
#undef PASTER
#undef EVALUATOR
#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define __overload(FUNC, TYPE) EVALUATOR(FUNC, TYPE)

#ifndef StackElementDump
#define StackElementDump(FILE, VALUE) {fprintf(FILE, "%d", VALUE);}
#endif

#ifndef StackElementType
#define StackElementType int
#endif

#ifndef StackRigid_h
#define StackRigid_h

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

static uint32_t adlerChecksum(const void* firstBlock, size_t len) {
    uint32_t a = 1, b = 0;
    const uint32_t MOD_ADLER = 65521;
    for (size_t index = 0; index < len; ++index) {
        a = (a + ((char*)firstBlock)[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    return (b << 16) | a;
}


/**
 * Codes returned after operations on Stack
 */
typedef enum StackRigidOperationCodes{
    /**
     * Succesful operation
     */
    STACK_OP_OK,
    /**
     * Tried to get value from the empty stack
     */
    STACK_OP_UNDERFLOW,
    /**
     * Stack overflow and new memory allocation failed
     */
    STACK_OP_OVERFLOW,
    /**
     * No memory for stack alocations
     */
    STACK_OP_NOMEMORY,
    /**
     * Stack memory was altered by some outer access
     */
    STACK_OP_INTEGRITYERR,
    /**
     * Found NULL pointer during the operation
     */
    STACK_OP_NULL
} StackRigidOperationCodes;


/**
 * Codes characterizing Stack state
 */
typedef enum StackRigidState{
    /**
     * Stack is running as expected
     */
    STACK_ST_OK,
    /**
     * Stack was altered by some outer access
     */
    STACK_ST_INTEGRITYERR,
    /**
     * Stack is uninitialized
     */
    STACK_ST_UNINITIALIZED,
    /**
     * Found NULL pointer during the operation
     */
    STACK_ST_NULL
} StackRigidState;


#endif /* StackRigid_h */


/**
 * The main structure. There's no definition in the header file in order to block direct parameters access
 */
typedef struct __overload(StackRigid,StackElementType)  __overload(StackRigid,StackElementType);


struct __overload(StackRigid,StackElementType) {
    int32_t checkSum;
    uint32_t checkSumVital;
    size_t capacity;
    size_t size;
    FILE* logFile;
    StackElementType data[1];
};


/**
 * @attention Not for the manual use!
 * @brief Updates Stack checksums. Does not take into account bytes inside the stack but outside of its last element.
 * @verbatim
 * Stack memory :  [ __stack area__ | value 1, value 2, ..., value n || ___garbage area___]
 * ____________________________________________________________________^ alterations in this area are not checked
 * @endverbatim
 * @param[in,out] stack Stack that checksums are needed to be updated
 */
static void __StackUpdateChecksum( __overload(StackRigid,StackElementType)* stack);


/**
 * @attention Not for the manual use!
 * @brief Claculates big Stack checksum. Uses Adler-32 method
 * @verbatim
 * Stack memory : [ checkSum checkSumVital capacity size logFile | value 1, value 2, ..., value n || ___garbage area___]
 * ________________from here ^__________________________________________________________^to here
 * @endverbatim
 * @param[in] stack Stack that checksums are needed to be updated
 */
static uint32_t __StackGetChecksum( __overload(StackRigid,StackElementType)* stack);


/**
 * @attention Not for the manual use!
 * @brief Uses Adler-32 method. Calculates Stack checksum only for the vital parameters.
 * This is necessary because __StackGetChecksum() relies on capacity and size. If they were changed, behaviour is undefined
 *
 * @verbatim
 * Stack memory : [ checkSum checkSumVital capacity size logFile | value 1, value 2, ..., value n || ___garbage area___]
 * ______________________________from here ^__________________^to here
 * @endverbatim
 * @param[in] stack Stack that checksums are needed to be updated
 */
static uint32_t __StackGetChecksumVital( __overload(StackRigid,StackElementType)* stack);


/**
 * @attention Not for the manual use!
 * @brief Realocates Stack if needed
 * @param[in,out] stack Stack to be realocated
 * @param[in] direction which direction realocations are available.  > 0 -> expand space if needed, < 0 -> shrink space if needed
 */
static StackRigidOperationCodes __StackRealocate( __overload(StackRigid,StackElementType)** stack, short int direction);


/**
 * @attention Not for the manual use!
 * @brief Calculates Adler-32 checksum from the firstBlock to the firstBlock + len
 * @param[in] firstBlock first block to include to the checksum
 * @param[in] len length of byte sequence
 * @return checksum
 */
static uint32_t adlerChecksum(const void* firstBlock, size_t len);


/**
 * Create new stack with pre-defined capacity
 * @param[in] capacity initial stack capacity. Set to 0 if yo want the stack to adopt automaticaly.
 * @param[in,out] logFile file for logging
 * @return new stack pointer
 */
 __overload(StackRigid,StackElementType)* __overload(NewStackRigid, StackElementType)(const size_t capacity, FILE* logFile);

/**
 * Perform all stack checks: checksums, general constrains.
 * @param[in] stack Stack to be validated
 * @return calculated state
 */
StackRigidState StackValidate( __overload(StackRigid,StackElementType)* stack);


/**
 * Push the value to the stack
 * The operation can perform realocations if there is not enough space
 * @param[in,out] stack Stack to be changed
 * @param[in] value value to be pushed
 * @return the outcome of the operation
 */
StackRigidOperationCodes StackPush( __overload(StackRigid,StackElementType)** stack, StackElementType value);

/**
 * Pop value from the end of the stack
 * The operation can perform realocations if there is too much space
 * @param[in,out] stack Stack to be distructed
 * @param[out] value value to be pushed
 * @return the outcome of the operation
 */
StackRigidOperationCodes StackPop( __overload(StackRigid,StackElementType)** stack, StackElementType* value);


/**
 * Free stack memory
 * @param[in,out] stack Stack to be distructed
 * @return the outcome of the operation
 */
StackRigidOperationCodes StackDestruct( __overload(StackRigid,StackElementType)** stack);


/**
 * Free stack memory
 * @param[out] stack Stack to be used
 * @return the outcome of the operation
 */
StackRigidOperationCodes StackBack( __overload(StackRigid,StackElementType)* stack, StackElementType* value);


/**
 * Dumps debug information about the stack.
 * @param[in] stack Stack to be dumped
 */
void StackDump( __overload(StackRigid,StackElementType)* stack);


/**
 * Memory alocated for the Stack
 * @param[in] stack Stack to be checked
 * @return alocated bytes
 */
size_t StackRigidMemoryUse( __overload(StackRigid,StackElementType)* stack);


StackRigidOperationCodes StackPush( __overload(StackRigid,StackElementType)** stack, StackElementType value){
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
    return STACK_OP_OK;
}


 __overload(StackRigid,StackElementType)* __overload(NewStackRigid, StackElementType)(const size_t capacity, FILE* logFile){
    const size_t memory = sizeof( __overload(StackRigid,StackElementType)) + (capacity - 1) * sizeof(StackElementType);
     __overload(StackRigid,StackElementType)* pointer = ( __overload(StackRigid,StackElementType)*)calloc(memory, 1);
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


StackRigidOperationCodes StackPop( __overload(StackRigid,StackElementType)** stack, StackElementType* value) {
    
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

    return STACK_OP_OK;
}


StackRigidOperationCodes StackBack( __overload(StackRigid,StackElementType)* stack, StackElementType* value){
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


StackRigidState StackValidate( __overload(StackRigid,StackElementType)* stack) {
 
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
    

    return STACK_ST_OK;
}


static void __StackUpdateChecksum( __overload(StackRigid,StackElementType)* stack) {
    assert(stack);

    stack->checkSumVital = __StackGetChecksumVital(stack);
    stack->checkSum = __StackGetChecksum(stack);

}


static uint32_t __StackGetChecksumVital( __overload(StackRigid,StackElementType)* stack) {
    assert(stack);
    
    void* firstBlock = (char*)stack + sizeof(stack->checkSum) + sizeof(stack->checkSumVital);
    
    const size_t memory = sizeof(stack->size) + sizeof(stack->capacity)+ sizeof(stack->logFile);
    if (stack->size > stack->capacity) {
        return 0;
    }
    return adlerChecksum(firstBlock, memory);
}


static uint32_t __StackGetChecksum( __overload(StackRigid,StackElementType)* stack) {
    assert(stack);
    
    void* firstBlock = (char*)stack + sizeof(stack->checkSum);
    
    // Memory used by Stack excluding checkSum and free space
    #ifndef DEBUG
        const size_t memory = StackRigidMemoryUse(stack) - sizeof(stack->checkSum) - (stack->capacity - stack->size) * sizeof(StackElementType);
    #endif
    #ifdef DEBUG
        const size_t memory = StackRigidMemoryUse(stack) - sizeof(stack->checkSum);
    #endif
    
    if (stack->size > stack->capacity) {
        return 0;
    }
    return adlerChecksum(firstBlock, memory);
}


static StackRigidOperationCodes __StackRealocate( __overload(StackRigid,StackElementType)** stack, short int direction) {
    if ((*stack)->capacity == 0) {
        (*stack)->capacity = 16;
        
        const size_t memory = StackRigidMemoryUse(*stack);
        
         __overload(StackRigid,StackElementType)* newStack = ( __overload(StackRigid,StackElementType)*) realloc((*stack), memory);
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
        const size_t memoryNew = sizeof(__overload(StackRigid,StackElementType)) + (newCapacity - 1) * sizeof(StackElementType);
        
        if (memoryNew >= memoryNow) {
             __overload(StackRigid,StackElementType)* newStack = ( __overload(StackRigid,StackElementType)*) realloc((*stack), memoryNew);
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
        const size_t memoryNew = sizeof(__overload(StackRigid,StackElementType)) + (newCapacity - 1) * sizeof(StackElementType);
        
        if (memoryNew <= memoryNow) {
             __overload(StackRigid,StackElementType)* newStack = ( __overload(StackRigid,StackElementType)*) realloc((*stack), memoryNew);
            if (newStack == NULL)
                return STACK_OP_NOMEMORY;
            
            (*stack) = newStack;
            (*stack)->capacity = newCapacity;
        }else
            return STACK_OP_OVERFLOW; // Exceded size_t memory
    }
    return STACK_OP_OK;
}

size_t StackRigidMemoryUse( __overload(StackRigid,StackElementType)* stack) {
    assert(stack);
    return sizeof(__overload(StackRigid,StackElementType)) + (stack->capacity - 1) * sizeof(StackElementType);
}


void StackDump( __overload(StackRigid,StackElementType)* stack) {
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
            fprintf(output, "*[%lu] : ", i);
            StackElementDump(stack->logFile, stack->data[i]);
            fprintf(output, "\n");
        }
        fprintf(output, "\t\t _______\n");
        if (stack->size < stack->capacity) {
            fprintf(output, "\t\t [%lu] : GARBAGE(", i + 1);
            StackElementDump(stack->logFile, stack->data[i + 1]);
            fprintf(output, ")\n");
        }
        if (stack->size + 1 < stack->capacity && stack->size + 1 > stack->size) {
            fprintf(output, "\t\t [%lu] : GARBAGE(", i + 2);
            StackElementDump(stack->logFile, stack->data[i + 2]);
            fprintf(output, ")\n");
        }
        fprintf(output, "\t}\n");
        fprintf(output, "}\n");
        
        const size_t memory = StackRigidMemoryUse(stack);
        fprintf(output, "Size allocated : %lu bytes\n", memory);
        fprintf(output, "Raw Stack size : %lu bytes\n", sizeof(__overload(StackRigid,StackElementType)));
        fprintf(output, "  Element size : %lu bytes\n", sizeof(StackElementType));
        fprintf(output, "     Block end : %p\n", (char*)stack + memory);
    }
    fprintf(output, "=================================\n");
}


StackRigidOperationCodes StackDestruct( __overload(StackRigid,StackElementType)** stack) {
    if (stack == NULL) {
        return STACK_OP_NULL;
    }
    if (*stack == NULL) {
        return STACK_OP_NULL;
    }
    free(*stack);
    return STACK_OP_OK;
}
