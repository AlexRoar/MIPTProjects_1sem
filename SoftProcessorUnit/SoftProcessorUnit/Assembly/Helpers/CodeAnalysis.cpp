//
//  CodeAnalysis.cpp
//  SPUAsm
//
//  Created by Александр Дремов on 15.10.2020.
//

#include "CodeAnalysis.hpp"
#include "CommandToBytecode.hpp"
#include <string.h>
#include "StringDistance.hpp"

static int isspace(unsigned char a){
    return a == ' ';
}


// Stores the trimmed input string into the given output buffer, which must be
// large enough to store the result.  If it is too small, the output is
// truncated.
size_t trimwhitespace(char *out, size_t len, const char *str)
{
  if(len == 0)
    return 0;

  const char *end;
  size_t out_size;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
  {
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len - 1 ? (end - str) : len-1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;

  return out_size;
}

int generateErrors(SyntaxMapping* mapping, AssemblyParams* params, char* code) {
    char* newInstruction = code;
    int result = 1;
    int lineNo = 0;
    while (newInstruction != NULL) {
        char* newlinePos = strchr(newInstruction + 1, '\n');
        if (newlinePos != NULL){
            *newlinePos = '\0';
        }
        result &= analyzeInstructionErrors(mapping, params, newInstruction + 1, ++lineNo);
        
        if (newlinePos != NULL){
            *newlinePos = '\n';
        }
        newInstruction = strchr(newInstruction + 1, '\n');
    }
    
    return result;
}

int analyzeInstructionErrors(SyntaxMapping* mapping, AssemblyParams* params, char* line, int lineNo) {
    size_t len = strlen(line);
    char* trimmed = (char*)calloc(len + 1, 1);
    len = trimwhitespace(trimmed, len + 1, line);
    
    
    if (trimmed[0] == ';' || len <= 1){
        free(trimmed);
        return 1;
    }
    
    int argc = 0;
    const SyntaxEntity* foundEntity = fetchCommand(mapping, trimmed);
    int argsLen[SPU_CMD_MAXARGS] = {0};
    const char** argv = getArgList(trimmed, &argc, argsLen);
    
    if (foundEntity == NULL) {
        const SyntaxEntity* best = bestMatchCommand(mapping, trimmed);
        fprintf(stderr, "%s:%d:1: error: unknown instruction found maybe you ment '%s'?\n%s\n",params->inputFileRealName, lineNo, best->naming, trimmed);
        for (int i = 1; i < len; i++) {
            if (trimmed[i] == ' ') {
                fprintf(stderr, "^\n");
                break;
            }
            fprintf(stderr, "~");
        }
        
        
        fprintf(stderr, "%s\n", best->naming);
        
        free(trimmed);
        free(argv);
        return 0;
    }
    
    int validArguments = isValidArgumentsNumber(foundEntity, argc - 1);
    
    if (validArguments != 1) {
        fprintf(stderr, "%s:%d:1: error: invalid arguments, expected: %s\n%s\n", params->inputFileRealName, lineNo, foundEntity->format, trimmed);
        for (int i = 0; i < argsLen[0]; i++) {
            fprintf(stderr, " ");
        }
        fprintf(stderr, " ^");
        for (int i = 1; i < SPU_CMD_MAXARGS; i++) {
            for (int j = 0; j < argsLen[i]; j++) {
                fprintf(stderr, "~");
            }
        }
        fprintf(stderr, "\n");
        free(trimmed);
        free(argv);
        return 0;
    }
    
    BinaryFile* binary = NewBinaryFile();
    CommandToBytesResult parseRes = foundEntity->cProcessor(foundEntity, params, binary, argc, argv);
    DestructBinaryFile(binary);
    
    switch (parseRes) {
        case SPU_CTB_ERROR:{
            fprintf(stderr, "%s:%d:1: error: assembly: general syntax error\n", params->inputFileRealName, lineNo);
            break;
        }
        case SPU_CTB_UNKNOWN_REGISTER:{
            fprintf(stderr, "%s:%d:1: error: assembly: unknown register\n", params->inputFileRealName, lineNo);
            break;
        }
        case SPU_CTB_INVALID_NUMBER:{
            fprintf(stderr, "%s:%d:1: error: assembly: invalid number of arguments\n", params->inputFileRealName, lineNo);
            break;
        }
        case SPU_CTB_OK:{
            break;
        }
    }
    
    free(trimmed);
    free(argv);
    return 1;
}

const SyntaxEntity* bestMatchCommand(SyntaxMapping* mapping, char* command) {
    char* nextInstr = strchr(command, ' ');
    if (nextInstr != NULL){
        *nextInstr = '\0';
    }
    
    int minValue = 0;
    int index = -1;
    for (int i = 0; i < mapping->number; i++) {
        edit *script;
        size_t distance = levenshtein_distance(mapping->entities[i].naming, command, &script);
        if (index == -1 || minValue > distance){
            minValue = (int)distance;
            index = i;
        }
    }
    if (nextInstr != NULL){
        *nextInstr = ' ';
    }
    
    return mapping->entities + index;
}
