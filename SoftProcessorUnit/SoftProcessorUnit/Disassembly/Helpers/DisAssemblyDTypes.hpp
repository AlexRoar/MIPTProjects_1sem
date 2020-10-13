//
//  DisAssemblyDTypes.h
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 13.10.2020.
//

#ifndef DisAssemblyDTypes_h
#define DisAssemblyDTypes_h

struct DisassemblyParams{
    FILE* inputFile;
    const char* inputFileName;
    
    FILE* outputFile;
    const char* outputFileName;
    
    int   verbose;
};

enum DisassemblyParseResult{
    SPU_DISASM_OK,
    SPU_DISASM_UNKNOWN_CMD,
    SPU_DISASM_WRONG_CMDFORMAT,
    SPU_DISASM_NOTENOUGHARGS
};

#endif /* DisAssemblyDTypes_h */
