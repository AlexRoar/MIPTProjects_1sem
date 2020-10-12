//
//  Syntax.c
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 12.10.2020.
//
#include "Syntax.hpp"
#include "string.h"

/**
 * push -> 0
 * pop -> 1
 * in -> 2
 * dump -> 3
 * add -> 4
 * sub -> 5
 * mul -> 6
 * div -> 7
 * sin -> 8
 * cos -> 9
 * sqrt -> 10
 * pow -> 11
 * het -> 12
 * out -> 13
 */

static SyntaxEntity makeEntity(const char* name, const char* format, const char flagBytes){
    static char code = 0;
    SyntaxEntity newEntity = {name, code++, format, flagBytes};
    return newEntity;
}


SyntaxMapping getSyntaxMapping(void) {
    static SyntaxEntity operations[] = {
        makeEntity("push", "*", 1),
        makeEntity("pop" , "?", 1),
        makeEntity("in"  , "?", 1),
        makeEntity("dump", "" , 0),
        
        makeEntity("add" , "" , 0),
        makeEntity("sub" , "" , 0),
        makeEntity("mul" , "" , 0),
        makeEntity("div" , "" , 0),
        
        makeEntity("sin" , "" , 0),
        makeEntity("cos" , "" , 0),
        makeEntity("sqrt", "" , 0),
        makeEntity("pow" , "" , 0),
        
        makeEntity("het" , "" , 0),
        makeEntity("out" , "" , 0),
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
