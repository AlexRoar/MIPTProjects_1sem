//
//  SPUDisAssembly.cpp
//  Disassembly
//
//  Created by Александр Дремов on 12.10.2020.
//

#include <stdio.h>
#include "AssemblyHelpers.hpp"
#include "DisAssemblyDTypes.hpp"
#include "DisAssemblyHelpers.hpp"


int main(int argc, const char **argv) {
    BinaryFile* binary = NewBinaryFile();
    DisassemblyParams parameters = {};
    
    int parsingResult = parseArgs(argc, argv, &parameters);
    
    if (parsingResult != EXIT_SUCCESS) {
        printf("disassembly: process finished with EXIT_FAILURE code\n");
        return EXIT_FAILURE;
    }
    
    BinFileLoadResult result = loadBinFile(binary, parameters.inputFile);
    
    if (result != SPU_BINLOAD_OK) {
        switch (result) {
            case SPU_BINLOAD_WRONG_VERSION:
                printf("disassembly: binary version is not aligned with current build\n");
                break;
                
            case SPU_BINLOAD_CORRUPTED:
                printf("disassembly: file structure corrupted\n");
                break;
            case SPU_BINLOAD_WRONG_SIGNATURE:
                printf("disassembly: file signature is wrong\n");
                break;
            case SPU_BINLOAD_OK:
                break;
        }
    } else {
        if (parameters.verbose) {
            char* SPUAssemblyVersion_chars = (char*)&(binary->version);
            printf("disassembly: successfully loaded file\n");
            printf("disassembly: specified version v%c.%c.%c%c\n",
                   SPUAssemblyVersion_chars[0],
                   SPUAssemblyVersion_chars[1],
                   SPUAssemblyVersion_chars[2],
                   SPUAssemblyVersion_chars[3]);
            
            printf("Scanned code:\n");
            for (size_t i = 0; i < binary->currentSize; i++) {
                printf("%d ", binary->code[i]);
            }
            printf("\n");
        }
    }
    
    SyntaxMapping syntax = getSyntaxMapping();
    
    disAssamblyCode(&parameters, &syntax, binary);

    DestructBinaryFile(binary);
    DestructDisAssemblyParams(&parameters);
    return 0;
}
