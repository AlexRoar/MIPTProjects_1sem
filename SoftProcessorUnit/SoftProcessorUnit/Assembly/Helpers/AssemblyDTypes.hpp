//
//  AssemblyDTypes.h
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 13.10.2020.
//

#ifndef AssemblyDTypes_h
#define AssemblyDTypes_h

struct AssemblyParams{
    FILE* inputFile;
    const char* inputFileName;
    
    FILE* outputFile;
    const char* outputFileName;
    
    FILE* lstFile;
    const char* lstFileName;
    
    FILE* prepFile;
    const char* prepFileName;
    
    int   verbose;
};

struct BinaryFile{
    short int  prepend;
    int        version;
    int        signature;
    size_t     stackSize;
    size_t     currentSize;
    
    // code
    char*      code;
    
    // unrecorded
    size_t     maxSize;
    size_t     codeOffset;
};

enum BinFileLoadResult{
    SPU_BINLOAD_OK,
    SPU_BINLOAD_WRONG_VERSION,
    SPU_BINLOAD_WRONG_SIGNATURE,
    SPU_BINLOAD_CORRUPTED
};


#endif /* AssemblyDTypes_h */
