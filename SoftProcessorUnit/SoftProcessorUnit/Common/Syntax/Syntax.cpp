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

static SyntaxEntity makeEntity(const char* name, const char code, const char* format, const char flagBytes,
                               OPTRANSLATE_FUNC((*cProcessor)),
                               OPBACKTRANSLATE_FUNC((*bProcessor))){
    SyntaxEntity newEntity = {name, code, format, flagBytes, cProcessor, bProcessor};
    return newEntity;
}


SyntaxMapping getSyntaxMapping(void) {
    static SyntaxEntity operations[] = {
        makeEntity("push",  0 , "*", 1, &cPush, &bPush),
        makeEntity("pop" ,  1 , "?", 1, &cPop , &bPop ),
        makeEntity("in"  ,  2 , "?", 1, &cIn  , &bIn  ),
        makeEntity("dump",  3 , "" , 0, &cDump, &bDump),
        makeEntity("clear", 4 , "" , 0, &cClear,&bClear),
        
        makeEntity("add" ,  5,  "" , 0, &cAdd , &bAdd ),
        makeEntity("sub" ,  6 , "" , 0, &cSub , &bSub ),
        makeEntity("mul" ,  7 , "" , 0, &cMul , &bMul ),
        makeEntity("div" ,  8 , "" , 0, &cDiv , &bDiv ),
        
        makeEntity("sin" ,  9 ,  "" , 0, &cSin , &bSin  ),
        makeEntity("cos" ,  10 , "" , 0, &cCos , &bCos  ),
        makeEntity("sqrt",  11 , "" , 0, &cSqrt, &bSqrt ),
        makeEntity("pow" ,  12 , "" , 0, &cPow , &bPow  ),
        
        makeEntity("het" ,  13 , "" , 0, &cHet , &bHet  ),
        makeEntity("out" ,  14 , "?", 1, &cOut , &bOut  ),
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
