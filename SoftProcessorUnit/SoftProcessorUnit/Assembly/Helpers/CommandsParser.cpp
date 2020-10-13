//
//  CommandsParser.cpp
//  SPUAsm
//
//  Created by Александр Дремов on 12.10.2020.
//

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "CommandsParser.hpp"
#include "AssemblyHelpers.hpp"
#include "Syntax.hpp"
#include "CommandsDTypes.hpp"


char* getSourceFileData(FILE* inputFile, size_t* length) {
    fseek(inputFile, 0, SEEK_END);
    *length = (size_t) ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);
    
    char *buffer = (char*)calloc(*length + 2, sizeof(char));
    if (buffer) {
        fread(buffer, 1, *length, inputFile);
        buffer[*length] = '\0';
    } else {
        return NULL;
    }
    
    return buffer;
}

void preprocessSource(char* code, size_t* length) {
    for (int i = 0; i + 1 < *length; ) {
        if(code[i] == ' ' && (code[i+1] == ' ' || code[i-1] == ' ')) {
            for(int j = i; j < *length; j++) {
                code[j]=code[j + 1];
            }
            (*length)--;
        } else {
            i++;
        }
    }
    
    char* commentPos = strchr(code, ';');
    while (commentPos != NULL) {
        char* commentPosNext = strchr(commentPos, '\n');
        if (commentPosNext == NULL){
            commentPosNext = code + *length - 1;
        }
        size_t emptyLength = 0;
        char* mover = commentPosNext;
        char* commentPosTmp = commentPos;
        while (mover != code + *length) {
            emptyLength++;
            *(commentPosTmp++) = *(mover++);
        }
        *(commentPosTmp) = '\0';
        *length = strlen(code);
        commentPos = strchr(commentPos + 1, ';');
    }
    
    for (int i = 0; i + 1 < *length; ) {
        if(code[i] == '\n' && (code[i+1] == '\n' || code[(i > 0) ? i - 1 : i] == '\n')) {
            for(int j = i; j < *length; j++) {
                code[j]=code[j + 1];
            }
            (*length)--;
        } else {
            i++;
        }
    }
}

const SyntaxEntity* fetchCommand(const SyntaxMapping* mapping, char* codeBlock) {
    char* command = (char*) calloc(strlen(codeBlock) + 1, sizeof(char));
    strcpy(command, codeBlock);
    char* firstWhitespace = strchr(command, ' ');
    if (firstWhitespace != NULL){
        *firstWhitespace = '\0';
    } else {
        firstWhitespace = strchr(command, '\n');
        if (firstWhitespace != NULL){
            *firstWhitespace = '\0';
        }
    }
    
    const SyntaxEntity* foundCommand = getSyntaxEntityByName(mapping, (const char*)command);
    
    free(command);
    return foundCommand;
}

const char** getArgList(char* codeBlock, int* argc) {
    *argc = 1;
    char** args = (char**) calloc(SPU_CMD_MAXARGS, sizeof(char*));
    char* firstWhitespace = strchr(codeBlock, ' ');
    args[0] = codeBlock;
    if(firstWhitespace == NULL)
        return (const char**)args;
    
    int argumentsAvailable = 1;
    if (firstWhitespace != NULL) {
        firstWhitespace = strchr(firstWhitespace, ' ');
        while (firstWhitespace != NULL) {
            if (!codeBlockEmpty(firstWhitespace)){
                args[argumentsAvailable] = firstWhitespace + 1;
                argumentsAvailable++;
            }
            firstWhitespace = strchr(firstWhitespace + 1, ' ');
        }
    }
    *argc = argumentsAvailable;
    return (const char**)args;
}

int isValidArgumentsNumber(const SyntaxEntity* mapping, int argc) {
    const char* formatPtr = mapping->format;
    
    int maxPossible = 0;
    int argumentsTotal = argc;
    int argumentsAvailable = argc;
    while (*formatPtr != '\0') {
        if (*formatPtr == '*' && argumentsAvailable <= 0)
            return 0;
        argumentsAvailable--;
        formatPtr++;
        maxPossible++;
    }
    
    if (argumentsTotal > maxPossible)
        return 0;
    
    return 1;
}

CommandParseResult parseCommand(AssemblyParams* compileParams, const SyntaxMapping* mapping, BinaryFile* binary, char* codeBlock) {
    char* newlinePos = strchr(codeBlock, '\n');
    if (newlinePos != NULL){
        *newlinePos = '\0';
    }
    
    const SyntaxEntity* foundEntity = fetchCommand(mapping, codeBlock);
    
    if (foundEntity == NULL){
        if (compileParams->verbose) {
            printf("assembly: unknown instruction '%s' found\n", codeBlock);
        }
        fprintf(compileParams->lstFile, "assembly: unknown instruction '%s' found\n", codeBlock);
        return SPU_UNKNOWN_COMMAND;
    }
    
    int argc = 0;
    const char** argv = getArgList( codeBlock, &argc);
    
    int validArguments = isValidArgumentsNumber(foundEntity, argc - 1);
    
    if (validArguments == 0){
        if (compileParams->verbose) {
            printf("assembly: wrong instruction '%s' found. "
                   "Arguments number is not valid. "
                   "Valid format: '%s'\n", codeBlock, foundEntity->format);
        }
        fprintf(compileParams->lstFile, "assembly: wrong instruction '%s' found. "
               "Arguments number is not valid. "
               "Valid format: '%s'\n", codeBlock, foundEntity->format);
        return SPU_CMD_WRONG_ARGUMENTS;
    }
    
    CommandToBytesResult parseRes = foundEntity->cProcessor(foundEntity, compileParams, binary, argc, argv);
    
    /**
     SPU_CTB_OK,
     SPU_CTB_ERROR,
     SPU_CTB_UNKNOWN_REGISTER,
     SPU_CTB_INVALID_NUMBER
     */
    switch (parseRes) {
        case SPU_CTB_ERROR:{
            printf("assembly: general syntax error\n");
            fprintf(compileParams->lstFile, "assembly: general syntax error\n");
            break;
        }
        case SPU_CTB_UNKNOWN_REGISTER:{
            printf("assembly: unknown register\n");
            fprintf(compileParams->lstFile, "assembly: unknown register\n");
            break;
        }
        case SPU_CTB_INVALID_NUMBER:{
            printf("assembly: invalid number of arguments\n");
            fprintf(compileParams->lstFile, "assembly: invalid number of arguments\n");
            break;
        }
        case SPU_CTB_OK:{
            break;
        }
    }

    if (parseRes != SPU_CTB_OK) {
        return SPU_FINALPARSE_ERROR;
    }
    
    if (newlinePos != NULL){
        *newlinePos = '\n';
    }
    free(argv);
    return SPU_PARSE_OK;
}

int codeBlockEmpty(char* codeBlock) {
    while (*codeBlock != '\0' && *codeBlock != '\n') {
        if (isprint(*codeBlock) && *codeBlock != ' ')
            return 0;
        codeBlock++;
    }
    return 1;
}

CommandParseResult parseCode(AssemblyParams* compileParams, const SyntaxMapping* mapping, BinaryFile* binary, char* code, size_t length) {
    char* lastBlockPos = code;
    
    size_t instrUct = 1;
    while (lastBlockPos != NULL && lastBlockPos != (char*)NULL + 1) {
        if (!codeBlockEmpty(lastBlockPos)) {
            CommandParseResult res = parseCommand(compileParams, mapping, binary, lastBlockPos);
            if (res != SPU_PARSE_OK){
                if (compileParams->verbose) {
                    printf("assembly: failed to parse instruction no. %zu '%s'\n", instrUct, lastBlockPos);
                }
                fprintf(compileParams->lstFile, "assembly: failed to parse instruction no. %zu '%s'\n", instrUct, lastBlockPos);
                return res;
            }
            instrUct++;
        }
        
        lastBlockPos = ((char*)memchr(lastBlockPos, '\n', length - (lastBlockPos - code))) + 1;
    }
    
    
    return SPU_PARSE_OK;
}

int registerNoFromName(char* name) {
    if (strcmp("rax", name) == 0) {
        return 0;
    }
    if (strcmp("rbx", name) == 0) {
        return 1;
    }
    if (strcmp("rcx", name) == 0) {
        return 2;
    }
    if (strcmp("rdx", name) == 0) {
        return 3;
    }
    return -1;
}

const char* registerNameFromNo(int no) {
    switch (no) {
        case 0:
            return "rax";
            break;
        case 1:
            return "rbx";
            break;
        case 2:
            return "rcx";
            break;
        case 3:
            return "rdx";
            break;
    }
    return "<UNKNOWN REGISTER>";
}


void codeEstimations(BinaryFile* binary, char* code){
    size_t stackInitSize = 0;
    char* p = strstr(code, "push");
    while(p != NULL){
        stackInitSize++;
        p = strstr(p + 1, "push");
    }
    binary->stackSize = stackInitSize;
}
