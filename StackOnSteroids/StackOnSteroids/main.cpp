//
//  main.cpp
//  StackOnSteroids
//
//  Created by Александр Дремов on 29.09.2020.
//

/**
 * Element type to be used in Stack
 */
#define  StackElementType int
#include "StackRigid.h"
#undef   StackElementType

#undef   StackElementDump
#define  StackElementDump(FILE, VALUE) {fprintf(FILE, "%g", VALUE);}
#define  StackElementType double
#include "StackRigid.h"

/**
 * Here, I try somehow to break the Stack
 */
int main(int argc, const char * argv[]) {
    StackRigid_int* newStack = NewStackRigid_int(1000, stdout);
    printf("  size_t volume: %lu bytes\n", sizeof(size_t));
    printf("checksum volume: %lu bytes\n", sizeof(uint32_t));
    for(size_t i = 0; i< 1000; i++){
        StackPush(&newStack, (int)i);
    }
    for(size_t i = 0; i < 1000; i++){
        int tmpVal;
        StackPop(&newStack, &tmpVal);
//        printf("Popped %d\n", tmpVal);
    }
    for(size_t i = 0; i < 100; i++){
        StackPush(&newStack, (int)i + 1);
    }
    
    StackRigid_double* newStackDouble = NewStackRigid_double(1000, stdout);
    for(size_t i = 0; i< 1000; i++){
        StackPush(&newStackDouble, (double)(i));
    }
    for(size_t i = 0; i < 1000; i++){
        double tmpVal;
        StackPop(&newStackDouble, &tmpVal);
//        printf("Popped %d\n", tmpVal);
    }
    for(size_t i = 0; i < 100; i++){
        StackPush(&newStackDouble, (double)i + 0.5);
    }
    
    for(size_t i = 0; i < StackRigidMemoryUse(newStack); i++){
        char previousValue = *((char*)newStack + i);
        for (char bullet = -128; bullet < 127; bullet++){
            if (bullet == previousValue)
                continue;
            *((char*)newStack + i) = bullet;
            StackRigidState state = StackValidate(newStack);
            if (state != STACK_ST_INTEGRITYERR){
                printf("Did not spot intervention at position %zu, byte %x\n", i, bullet);
                break;
            }
            *((char*)newStack + i) = previousValue;
        }
    }

    StackDump(newStack);
    StackDump(newStackDouble);
    StackDestruct(&newStack);
    return 0;
}
