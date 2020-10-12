//
//  CommandsParser.hpp
//  SPUAsm
//
//  Created by Александр Дремов on 12.10.2020.
//

#ifndef CommandsParser_hpp
#define CommandsParser_hpp

#include <stdio.h>
#include "Syntax.hpp"
#include "AssemblyHelpers.hpp"

enum CommandParseResult {
    SPU_PARSE_OK,
    SPU_CMD_WRONG_ARGUMENTS,
    SPU_UNKNOWN_COMMAND
};

char* getSourceFileData(FILE* inputFile, size_t* length);

void preprocessSource(char* code, size_t* length);

int codeBlockEmpty(char* codeBlock);

CommandParseResult parseCommand(AssemblyParams* compileParams, const SyntaxMapping* mapping, BinaryFile* binary, char* codeBlock);

CommandParseResult parseCode(AssemblyParams* compileParams, const SyntaxMapping* mapping, BinaryFile* binary, char* code, size_t length);

const SyntaxEntity* fetchCommand(const SyntaxMapping* mapping, char* codeBlock);

int isValidArgumentsNumber(const SyntaxEntity* mapping, char* codeBlock, int* hasArguments);

#endif /* CommandsParser_hpp */
