//
//  main.cpp
//  StackOnSteroids
//
//  Created by Александр Дремов on 29.09.2020.
//
#include "StackRigid.h"
extern double StackRigidAvgTimes[5];

int main(int argc, const char * argv[]) {
    StackRigid* newStack = NewStackRigid(1000, stdout);
    printf("  size_t volume: %lu bytes\n", sizeof(size_t));
    printf("checksum volume: %lu bytes\n", sizeof(uint32_t));
    for(size_t i = 0; i< 1000; i++){
        StackPush(&newStack, (int)i);
    }
    for(size_t i = 0; i < 1000; i++){
        int tmpVal;
        StackPop(&newStack, &tmpVal);
    }
    for(size_t i = 0; i < 100; i++){
        StackPush(&newStack, (int)i);
    }
    
    for(size_t i = 0; i < StackRigidMemoryUse(newStack); i++){
        char previousValue = *((char*)newStack + i);
        for (char bullet = -128; bullet < 127; bullet++){
            if (bullet == previousValue)
                continue;
            *((char*)newStack + i) = bullet;
            StackRigidState state = StackValidate(newStack);
            if (state != STACK_ST_INTEGRITYERR){
                printf("Did not spot at position %zu, byte %x\n", i, bullet);
                break;
            }
            *((char*)newStack + i) = previousValue;
        }
    }
    
    printf("\nAverage checksum update time: %g s\n", StackRigidAvgTimes[0]);
    printf("Average validation time: %g s\n", StackRigidAvgTimes[1]);
    printf("Average push time: %g s\n", StackRigidAvgTimes[2]);
    printf("Average pop time: %g s\n", StackRigidAvgTimes[3]);
    printf("Average reallocation time: %g s\n", StackRigidAvgTimes[4]);
    StackDestruct(&newStack);
    return 0;
}

#undef DEBUGSPEED
