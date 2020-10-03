//
//  main.cpp
//  StackOnSteroids
//
//  Created by Александр Дремов on 29.09.2020.
//

/**
 * Element type to be used in Stack
 */
#define StackElementType int
#include "StackRigid.h"
#undef StackElementType

#undef StackElementDump
#define StackElementDump(FILE, VALUE) {fprintf(FILE, "%g", VALUE);}
#define StackElementType double
#include "StackRigid.h"

/**
 * Here, I try somehow to break the Stack
 */
int main(int argc, const char * argv[]) {
    StackRigid_int* newStack = NewStackRigid_int(1000, stdout);
    printf("  size_t volume: %lu bytes\n", sizeof(size_t));
    printf("checksum volume: %lu bytes\n", sizeof(uint32_t));
    for(size_t i = 0; i< 1000; i++){
        StackPush_int(&newStack, (int)i);
    }
    for(size_t i = 0; i < 1000; i++){
        int tmpVal;
        StackPop_int(&newStack, &tmpVal);
//        printf("Popped %d\n", tmpVal);
    }
    for(size_t i = 0; i < 100; i++){
        StackPush_int(&newStack, (int)i);
    }
    
    StackRigid_double* newStackDouble = NewStackRigid_double(1000, stdout);
    for(size_t i = 0; i< 1000; i++){
        StackPush_double(&newStackDouble, (double)(i));
    }
    for(size_t i = 0; i < 1000; i++){
        double tmpVal;
        StackPop_double(&newStackDouble, &tmpVal);
//        printf("Popped %d\n", tmpVal);
    }
    for(size_t i = 0; i < 100; i++){
        StackPush_double(&newStackDouble, (double)i/(double)rand());
    }
    
    for(size_t i = 0; i < StackRigidMemoryUse_int(newStack); i++){
        char previousValue = *((char*)newStack + i);
        for (char bullet = -128; bullet < 127; bullet++){
            if (bullet == previousValue)
                continue;
            *((char*)newStack + i) = bullet;
            StackRigidState state = StackValidate_int(newStack);
            if (state != STACK_ST_INTEGRITYERR){
                printf("Did not spot intervention at position %zu, byte %x\n", i, bullet);
                break;
            }
            *((char*)newStack + i) = previousValue;
        }
    }

    StackDump_int(newStack);
    StackDump_double(newStackDouble);
    StackDestruct_int(&newStack);
    return 0;
}
