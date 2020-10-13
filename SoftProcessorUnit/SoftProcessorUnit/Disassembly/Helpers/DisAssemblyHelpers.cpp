//
//  DisAssemblyHelpers.cpp
//  SPUDisAsm
//
//  Created by Александр Дремов on 13.10.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DisAssemblyHelpers.hpp"
#include "AssemblyHelpers.hpp"
#include "CommandsParser.hpp"
#include "SPUVersion.hpp"
#include "Syntax.hpp"


int parseArgs(int argc, const char* argv[], DisassemblyParams* params) {
    DisassemblyParams newParams = {0, 0, 0, 0, 0};
    if (argc <= 1){
        printDisAssemblyHelpData();
    }

    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0) {
            if (i + 1 > argc){
                printf("disassembly: No input file specified after --input\n");
                return EXIT_FAILURE;
            }
            FILE* inputFile = fopen(argv[i + 1], "rb");
            newParams.inputFile = inputFile;
            newParams.inputFileName = *(argv + i + 1);
            if (newParams.inputFile == NULL){
                printf("disassembly: Can't open input file\n");
                return EXIT_FAILURE;
            }
            i++;
        }else if (strcmp(argv[i], "--output") == 0) {
            if (i + 1 > argc){
                printf("disassembly: No output file specified after --output\n");
                return EXIT_FAILURE;
            }
            FILE* outputFile = fopen(argv[i + 1], "wb");
            newParams.outputFile = outputFile;
            newParams.outputFileName = *(argv + i + 1);
            i++;
        }else if (strcmp(argv[i], "--verbose") == 0) {
            newParams.verbose = 1;
        }else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printDisAssemblyHelpData();
        }
    }
    
    if (newParams.inputFile == NULL) {
        printf("disassembly: No input file specified\n");
        return EXIT_FAILURE;
    }
    
    if (newParams.outputFile == NULL) {
        newParams.outputFile = fopen("disasm.spus", "wb");
        newParams.outputFileName = "disasm.spus";
        if (newParams.outputFile == NULL){
            printf("disassembly: No output file specified\n");
            return EXIT_FAILURE;
        }
    }
    
    *params = newParams;
    
    return EXIT_SUCCESS;
}


void printDisAssemblyHelpData(void) {
    int SPUAssemblyVersion = SPU_VERSION;
    char* SPUAssemblyVersion_chars = (char*)&SPUAssemblyVersion;
    printf("SPUDisAssembly v%c.%c.%c%c help\n"
           "--input     <input file> input file to be assembled .spub format (spu binary)\n"
           "--output    <output file> output file. disasm.spus by default (spu source)\n"
           "-h, --help  show this help message\n"
           "--verbose   output disassembly debug information to the console\n"
           "\n",
           SPUAssemblyVersion_chars[0],
           SPUAssemblyVersion_chars[1],
           SPUAssemblyVersion_chars[2],
           SPUAssemblyVersion_chars[3]);
}

void DestructDisAssemblyParams(DisassemblyParams* params) {
    fclose(params->inputFile);
    fclose(params->outputFile);
}

DisassemblyParseResult disAssamblyCode(DisassemblyParams* params,
                                       struct SyntaxMapping* mapping,
                                       BinaryFile* binary) {
    char* SPI = binary->code;
    
    while(SPI < binary->code + binary->currentSize) {
        DisassemblyParseResult cmdRes = disAssamblyInstruction(params, mapping, binary, &SPI);
        if (cmdRes != SPU_DISASM_OK) {
            return cmdRes;
        }
    }
    
    return SPU_DISASM_OK;
}

DisassemblyParseResult disAssamblyInstruction(DisassemblyParams* params,
                                              struct SyntaxMapping* mapping,
                                              BinaryFile* binary,
                                              char** instruction) {
    char instructionId = **instruction;
    
    const SyntaxEntity* entity = getSyntaxEntityByCode(mapping, instructionId);
    if (entity == NULL) {
        return SPU_DISASM_UNKNOWN_CMD;
    }
    
    entity->bProcessor(entity,
                       params,
                       binary,
                       instruction);
    
    return SPU_DISASM_OK;
}
