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
#include "CodeAnalysis.hpp"


int main(int argc, const char* argv[]){
    int SPUAssemblyVersion = SPU_VERSION;
    SyntaxMapping syntax = getSyntaxMapping();
    AssemblyParams compileParams = {};
    
    int parsingResult = parseArgs(argc, argv, &compileParams);
    if (parsingResult != EXIT_SUCCESS) {
        printf("error: assembly: process finished with EXIT_FAILURE code\n");
        return EXIT_FAILURE;
    }
    
    char* SPUAssemblyVersion_chars = (char*)&SPUAssemblyVersion;
    if (compileParams.verbose) {
        
        printf("SPUAssembly v%c.%c.%c%c\n",
               SPUAssemblyVersion_chars[0],
               SPUAssemblyVersion_chars[1],
               SPUAssemblyVersion_chars[2],
               SPUAssemblyVersion_chars[3]);
    }
    
    if (compileParams.lstFile != NULL) {
        fprintf(compileParams.lstFile,
                "SPUAssembly v%c.%c.%c%c ",
                SPUAssemblyVersion_chars[0],
                SPUAssemblyVersion_chars[1],
                SPUAssemblyVersion_chars[2],
                SPUAssemblyVersion_chars[3]);
        fprintf(compileParams.lstFile, "%s -> %s\n", compileParams.inputFileName , compileParams.outputFileName);
    }

    size_t codeLen = 0;
    char* code = getSourceFileData(compileParams.inputFile, &codeLen);
    compileParams.codeText = code;
    
    removeDoubleWhitespaces(compileParams.codeText, &codeLen);
    
    int errorsFound = generateErrors(&syntax, &compileParams, compileParams.codeText);
    if (errorsFound == 0){
        free(code);
        DestructAssemblyParams(&compileParams);
        return EXIT_FAILURE;
    }
    
    preprocessSource(code, &codeLen);
    
    if (compileParams.prepFile != NULL) {
        fwrite(code, 1, codeLen,compileParams.prepFile);
    }
    
    BinaryFile* binary = NewBinaryFile();
//    delete compileParams.labelsStore;
//    compileParams.labelsStore = new La
    CommandParseResult parseRes = parseCode(&compileParams, (const SyntaxMapping*) &syntax, binary, compileParams.codeText, codeLen);
    
    LabelParse labelsRes = evaluateLabels(&compileParams, binary);
    
    if (parseRes != SPU_PARSE_OK || labelsRes != SPU_LABEL_OK){
        printf("%s: error: assembly: syntax error\n", compileParams.inputFileName);
        printf("%s: error: assembly: process finished with EXIT_FAILURE code\n", compileParams.inputFileName);
        return EXIT_FAILURE;
    }
    
    if (compileParams.verbose){
        printf("Generated code: \n");
        for (size_t i = 0; i < binary->currentSize; i++) {
            printf("%d ", (unsigned char)binary->code[i]);
        }
        printf("\n");
    }
    
    codeEstimations(binary, code);
    flushBinFile(binary, compileParams.outputFile);
    
    free(code);
    DestructBinaryFile(binary);
    DestructAssemblyParams(&compileParams);
}
