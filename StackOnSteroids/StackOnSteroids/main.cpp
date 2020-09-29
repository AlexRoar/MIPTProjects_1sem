//
//  main.cpp
//  StackOnSteroids
//
//  Created by Александр Дремов on 29.09.2020.
//

#include "StackRigid.h"

int main(int argc, const char * argv[]) {
    StackRigid* newStack = NewStackRigid(1);
    StackDump(stdout, newStack);
}
