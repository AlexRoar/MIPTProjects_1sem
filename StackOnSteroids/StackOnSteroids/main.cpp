//
//  main.cpp
//  StackOnSteroids
//
//  Created by Александр Дремов on 29.09.2020.
//

#include "StackRigid.h"
extern double StackRigidAvgTimes[5];

int main(int argc, const char * argv[]) {
    StackRigid* newStack = NewStackRigid(1000);
    for(size_t i = 0; i< 1000; i++){
        StackPush(&newStack, (int)i);
    }
    for(size_t i = 0; i < 1000; i++){
        int tmpVal;
        StackPop(&newStack, &tmpVal);
    }
    for(size_t i = 0; i < 1000; i++){
        StackPush(&newStack, (int)i);
    }
    
    short int outOfBoundary = 0;
    for(size_t i = 0; i < StackRigidMemoryUse(newStack); i++){
        char previousValue = *((char*)newStack + i);
        *((char*)newStack + i) = rand();
        StackRigidState state = StackValidate(newStack);
        if (state != STACK_ST_INTEGRITYERR && outOfBoundary == 0){
            printf("Did not spot at position %zu\n", i);
            outOfBoundary = 1;
            StackDump(stdout, newStack);
        }
        *((char*)newStack + i) = previousValue;
    }
    
    printf("Average checksum update time: %g s\n", StackRigidAvgTimes[0]);
    printf("Average validation time: %g s\n", StackRigidAvgTimes[1]);
    printf("Average push time: %g s\n", StackRigidAvgTimes[2]);
    printf("Average pop time: %g s\n", StackRigidAvgTimes[3]);
}

#undef DEBUGSPEED
