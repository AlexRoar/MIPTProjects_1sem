//
//  SPUAssembly.cpp
//  Assembly
//
//  Created by Александр Дремов on 12.10.2020.
//

#include <stdio.h>
#include "Syntax.hpp"
#include "AssemblyHelpers.hpp"
#include "CommandsParser.hpp"
#include "SPUVersion.hpp"


int main(int argc, const char* argv[]){
    int SPUAssemblyVersion = SPU_VERSION;
    SyntaxMapping syntax = getSyntaxMapping();
    AssemblyParams compileParams = {};
    
    int parsingResult = parseArgs(argc, argv, &compileParams);
    if (parsingResult != EXIT_SUCCESS) {
        printf("assembly: process finished with EXIT_FAILURE code\n");
        return EXIT_FAILURE;
    }
    
    if (compileParams.verbose) {
        char* SPUAssemblyVersion_chars = (char*)&SPUAssemblyVersion;
        printf("SPUAssembly v%c.%c.%c%c\n",
               SPUAssemblyVersion_chars[0],
               SPUAssemblyVersion_chars[1],
               SPUAssemblyVersion_chars[2],
               SPUAssemblyVersion_chars[3]);
        fprintf(compileParams.lstFile,
                "SPUAssembly v%c.%c.%c%c\n",
                SPUAssemblyVersion_chars[0],
                SPUAssemblyVersion_chars[1],
                SPUAssemblyVersion_chars[2],
                SPUAssemblyVersion_chars[3]);
    }

    size_t codeLen = 0;
    char* code = getSourceFileData(compileParams.inputFile, &codeLen);
    preprocessSource(code, &codeLen);
    
    BinaryFile* binary = NewBinaryFile();
    
    CommandParseResult parseRes = parseCode(&compileParams, (const SyntaxMapping*) &syntax, binary, code, codeLen);
    if (parseRes != SPU_PARSE_OK){
        printf("assembly: syntax error\n");
        printf("assembly: process finished with EXIT_FAILURE code\n");
        return EXIT_FAILURE;
    }
    
    free(code);
    DestructBinaryFile(binary);
    DestructAssemblyParams(&compileParams);
}
