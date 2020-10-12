//
//  AssemblyHelpers.hpp
//  SPUAsm
//
//  Created by Александр Дремов on 12.10.2020.
//

#ifndef AssemblyHelpers_hpp
#define AssemblyHelpers_hpp
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    FILE* inputFile;
    FILE* outputFile;
    FILE* lstFile;
    int   verbose;
} AssemblyParams;

typedef struct {
    short int  prepend;
    int        signature;
    int        version;
    char*      code;
    size_t     maxSize;
    size_t     currentSize;
} BinaryFile;

int parseArgs(int argc, const char* argv[], AssemblyParams* params);

BinaryFile* NewBinaryFile();

int appendToBinFile(BinaryFile* binFile, void* block, size_t size);

int resizeBinFile(BinaryFile* binFile, size_t spaceNeeded);

int commandParse (BinaryFile* binFile, char* command);

void DestructBinaryFile(BinaryFile* binFile);

void DestructAssemblyParams(AssemblyParams* params);

void printHelpData(void);

#endif /* AssemblyHelpers_hpp */
