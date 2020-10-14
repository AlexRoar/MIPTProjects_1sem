//
//  SPU.c
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 12.10.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "SPUCore.hpp"
#include "SPUCoreHelpers.hpp"
#include "Syntax.hpp"


int main(int argc, const char * argv[]) {
    RunParameters params  = {};
    
    int parseRes = parseSPUArgs(&params, argc, argv);
    if (parseRes != EXIT_SUCCESS) {
        fprintf(stderr, "spu: execution args parse terminated\n");
        return EXIT_FAILURE;
    }
    
    BinaryFile* binary = NewBinaryFile();
    BinFileLoadResult loadResult = loadBinFile(binary, params.inputFile);
    
    if (loadResult != SPU_BINLOAD_OK){
        switch (loadResult) {
            case SPU_BINLOAD_WRONG_VERSION:
                printf("spu: binary version is not aligned with current build\n");
                break;
                
            case SPU_BINLOAD_CORRUPTED:
                printf("spu: file structure corrupted\n");
                break;
            case SPU_BINLOAD_WRONG_SIGNATURE:
                printf("spu: file signature is wrong\n");
                break;
            case SPU_BINLOAD_OK:
                break;
        }
        DestructBinaryFile(binary);
        DestructSPUArgs(&params);
        return EXIT_FAILURE;
    }
    
    if (params.verbose) {
        printf("spu: loaded %s\n", params.inputFileName);
    }
    
    
    SPUCore core = {};
    
    ConstructSPUCore(&core, binary->stackSize);
    
    InstructionExeResult result = runCode(&core, &params, binary);
    
    if (result != SPU_EXE_OK) {
        switch (result) {
            case SPU_EXE_OK:
                break;
            case SPU_EXE_STACK_UNDERFLOW:
                printf("spu: stack underflow detected\n");
                break;
            case SPU_EXE_STACK_OVERFLOW:
                printf("spu: stack owerflow detected\n");
                break;
            case SPU_EXE_CORRUPTED:
                printf("spu: binary structure invalid\n");
                break;
            case SPU_EXE_ZERODIV:
                printf("spu: division by zero\n");
                break;
            case SPU_EXE_UNKNOWN:
                printf("spu: unknown command\n");
                break;
            case SPU_EXE_NOARGS:
                printf("spu: corrupted file, wrong instruction args\n");
                break;
            case SPU_EXE_NOMEMORY:
                printf("spu: run out of memory\n");
                break;
            case SPU_EXE_INVALIDINPUT:
                printf("spu: invalid input\n");
                break;
        }
    }
    
    if (params.verbose) {
        printf("spu: output finished to %s\n", params.outputFileName);
    }
    
    DestructSPUArgs(&params);
    DestructBinaryFile(binary);
    return EXIT_SUCCESS;
}
