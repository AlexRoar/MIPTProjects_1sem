//
//  StackRigid.h
//  StackOnSteroids
//
//  Created by Александр Дремов on 29.09.2020.
//

#ifndef StackRigid_h
#define StackRigid_h

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

typedef int StackElementType;
typedef struct StackRigid StackRigid;

typedef enum StackRigidOperationCodes{
    STACK_OP_OK, STACK_OP_UNDERFLOW, STACK_OP_OVERFLOW, STACK_OP_NOMEMORY
} StackRigidOperationCodes;

typedef enum StackRigidState{
    STACK_ST_OK, STACK_ST_INTEGRITYERR, STACK_ST_UNINITIALIZED, STACK_ST_NULL
} StackRigidState;

StackRigid* NewStackRigid(const size_t capacity);

StackRigidState StackValidate(StackRigid* stack);

StackRigidOperationCodes StackPush(StackRigid** stack, StackElementType value);

void StackDump(FILE* output, StackRigid* stack);

static void __StackUpdateChecksum(StackRigid* stack);

static uint32_t __StackGetChecksum(StackRigid* stack);

static void __StackRealocate(StackRigid** stack);

static uint32_t adlerChecksum(const void* firstBlock, size_t len);

#endif /* StackRigid_h */
