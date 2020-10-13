//
//  Syntax.c
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 12.10.2020.
//
#include "Syntax.hpp"
#include "CommandToBytecode.hpp"
#include "AssemblyHelpers.hpp"
#include "string.h"

static SyntaxEntity makeEntity(const char* name, const char* format, const char flagBytes,
                               OPTRANSLATE_FUNC((*cProcessor)),
                               OPBACKTRANSLATE_FUNC((*bProcessor))){
    static char code = 0;
    SyntaxEntity newEntity = {name, code++, format, flagBytes, cProcessor, bProcessor};
    return newEntity;
}


SyntaxMapping getSyntaxMapping(void) {
    static SyntaxEntity operations[] = {
        makeEntity("push", "*", 1, &cPush, &bPush),
        makeEntity("pop" , "?", 1, &cPop , &bPop ),
        makeEntity("in"  , "?", 1, &cIn  , &bIn  ),
        makeEntity("dump", "" , 0, &cDump, &bDump),
        
        makeEntity("add" , "" , 0, &cAdd , &bAdd ),
        makeEntity("sub" , "" , 0, &cSub , &bSub ),
        makeEntity("mul" , "" , 0, &cMul , &bMul ),
        makeEntity("div" , "" , 0, &cDiv , &bDiv ),
        
        makeEntity("sin" , "" , 0, &cSin , &bSin  ),
        makeEntity("cos" , "" , 0, &cCos , &bCos  ),
        makeEntity("sqrt", "" , 0, &cSqrt, &bSqrt ),
        makeEntity("pow" , "" , 0, &cPow , &bPow  ),
        
        makeEntity("het" , "" , 0, &cHet , &bHet  ),
        makeEntity("out" , "" , 0, &cOut , &bOut  ),
    };
    static SyntaxMapping mapping = {operations, sizeof(operations) / sizeof(SyntaxEntity)};
    
    return mapping;
}


int getSyntaxCode(const SyntaxMapping* mapping, const char* name) {
    for(int i = 0; i < mapping->number; i++){
        if (strcmp(mapping->entities[i].naming, name) == 0){
            return mapping->entities[i].code;
        }
    }
    return -1;
}


const char* getSyntaxName(const SyntaxMapping* mapping, const char code) {
    for(int i = 0; i < mapping->number; i++){
        if (code == mapping->entities[i].code){
            return mapping->entities[i].naming;
        }
    }
    return "";
}

const SyntaxEntity* getSyntaxEntityByCode(const SyntaxMapping* mapping, const char code) {
    for(int i = 0; i < mapping->number; i++){
        if (code == mapping->entities[i].code){
            return &mapping->entities[i];
        }
    }
    return NULL;
}

const SyntaxEntity* getSyntaxEntityByName(const SyntaxMapping* mapping, const char* name) {
    for(int i = 0; i < mapping->number; i++){
        if (strcmp(mapping->entities[i].naming, name) == 0){
            return &mapping->entities[i];
        }
    }
    return NULL;
}
