//
//  SPU.c
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 12.10.2020.
//

#ifndef DEBUG
    #define IGNORE_VALIDITY
#endif

#define StackElementDump(FILE, VALUE) {fprintf(FILE, "%g", VALUE);}
#define StackElementType double

#include <stdio.h>
#include <stdlib.h>
#include "StackRigid.h"
#include "Syntax.hpp"


int main(int argc, const char * argv[]) {
    SyntaxMapping mapping = getSyntaxMapping();
    
//    for (size_t i = 0; i < mapping.number; i++){
//        printf("%5s -> %d\n", mapping.entities[i].naming, mapping.entities[i].code);
//    }
}
