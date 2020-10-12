//
//  AssemblyHelpers.cpp
//  SPUAsm
//
//  Created by Александр Дремов on 12.10.2020.
//

#include <string.h>
#include <assert.h>
#include "AssemblyHelpers.hpp"
#include "SPUVersion.hpp"

int parseArgs(int argc, const char* argv[], AssemblyParams* params) {
    AssemblyParams newParams = {0, 0, 0, 0};
    
    if (argc <= 1){
        printHelpData();
    }

    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0) {
            if (i + 1 > argc){
                printf("assembly: No input file specified after --input\n");
                return EXIT_FAILURE;
            }
            FILE* inputFile = fopen(argv[i + 1], "rb");
            newParams.inputFile = inputFile;
            if (newParams.inputFile == NULL){
                printf("assembly: Can't open input file\n");
                return EXIT_FAILURE;
            }
            i++;
        }else if (strcmp(argv[i], "--output") == 0) {
            if (i + 1 > argc){
                printf("assembly: No output file specified after --output\n");
                return EXIT_FAILURE;
            }
            FILE* outputFile = fopen(argv[i + 1], "wb");
            newParams.outputFile = outputFile;
            i++;
        }else if (strcmp(argv[i], "--verbose") == 0) {
            newParams.verbose = 1;
        }else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printHelpData();
        }else if (strcmp(argv[i], "--lstfile") == 0) {
            if (i + 1 > argc){
                printf("assembly: No lstfile file specified after --lstfile\n");
                return EXIT_FAILURE;
            }
            FILE* lstFile = fopen(argv[i + 1], "wb");
            newParams.lstFile = lstFile;
            i++;
        }
    }
    if (newParams.inputFile == NULL){
        printf("assembly: No input file specified\n");
        return EXIT_FAILURE;
    }
    if (newParams.outputFile == NULL){
        newParams.outputFile = fopen("output.spub", "wb");
        if (newParams.outputFile == NULL){
            printf("assembly: No output file specified\n");
            return EXIT_FAILURE;
        }
    }
    if (newParams.lstFile == NULL){
        newParams.lstFile = fopen("assembly.lst", "wb");
        if (newParams.lstFile == NULL){
            printf("assembly: No lstFile file specified\n");
            return EXIT_FAILURE;
        }
    }
    *params = newParams;
    return EXIT_SUCCESS;
}


BinaryFile* NewBinaryFile(){
    BinaryFile* newBin = (BinaryFile*)calloc(1, sizeof(BinaryFile));
    newBin->code = (char*)calloc(256, sizeof(char));
    
    newBin->currentSize = 0;
    newBin->maxSize     = 256;
    newBin->prepend     = SPU_BIN_PREPEND;
    newBin->version     = SPU_VERSION;
    newBin->signature   = SPU_SIGNATURE;
    return newBin;
}


int appendToBinFile(BinaryFile* binFile, void* block, size_t size) {
    int resizeResult = resizeBinFile(binFile, size + 1);
    if (resizeResult != EXIT_SUCCESS)
        return resizeResult;
    
    memcpy(binFile->code + binFile->currentSize, block, size);
    
    return EXIT_SUCCESS;
}


int resizeBinFile(BinaryFile* binFile, size_t spaceNeeded){
    if (binFile->currentSize + spaceNeeded >= binFile->maxSize){
        char* newCode = (char*) realloc(binFile->code, binFile->maxSize + spaceNeeded);
        if (newCode == NULL)
            return EXIT_FAILURE;
        binFile->code = newCode;
        binFile->maxSize += spaceNeeded;
    }
    return EXIT_SUCCESS;
}


void printHelpData(void) {
    int SPUAssemblyVersion = SPU_VERSION;
    char* SPUAssemblyVersion_chars = (char*)&SPUAssemblyVersion;
    printf("SPUAssembly v%c.%c.%c%c help\n"
           "--input     <input file> input file to be assembled .spus format (spu source)\n"
           "--output    <output file> output file. output.spub by default (spu binary)\n"
           "-h, --help  show this help message\n"
           "--verbose   output assembly debug information to the console\n"
           "--lstfile   <.lst file> file to output .lst assembly data assembly.lst by default\n"
           "\n",
           SPUAssemblyVersion_chars[0],
           SPUAssemblyVersion_chars[1],
           SPUAssemblyVersion_chars[2],
           SPUAssemblyVersion_chars[3]);
}


void DestructBinaryFile(BinaryFile* binFile) {
    assert(binFile);
    free(binFile->code);
    free(binFile);
}

void DestructAssemblyParams(AssemblyParams* params) {
    fclose(params->inputFile);
    fclose(params->lstFile);
    fclose(params->outputFile);
}
