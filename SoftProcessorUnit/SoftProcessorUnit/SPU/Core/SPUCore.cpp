//
//  SPUCore.cpp
//  SPU
//
//  Created by Александр Дремов on 14.10.2020.
//

#include "SPUCore.hpp"

#define CALLEXE(name) return name (core,  params, binary, SPI)

void ConstructSPUCore(SPUCore* core, size_t initStackSize) {
    core->stack = NewStackRigid_double(initStackSize, stderr);
    core->terminated = 0;
}

InstructionExeResult runCode(SPUCore* core, RunParameters* params, BinaryFile* binary) {
    core->terminated = 0;
    
    char* SPI = binary->code;
    
    while (!core->terminated && SPI < binary->code + binary->currentSize) {
        char* prevOffset = SPI;
        InstructionExeResult cmdResult = executeInstruction(core, params, binary, &SPI);
        if (cmdResult != SPU_EXE_OK){
            core->terminated = 1;
            fprintf(stderr, "spu: terminated at offset: %d reached: %d\n",
                    (int)(prevOffset - binary->code + binary->codeOffset), (int)(SPI - binary->code + binary->codeOffset));
            return cmdResult;
        }
    }
    
    return SPU_EXE_OK;
}


InstructionExeResult executeInstruction(SPUCore* core,
                                        RunParameters* params,
                                        BinaryFile* binary,
                                        char** SPI) {
    
    switch (**SPI) {
        case 0:
            CALLEXE(ePush);
        case 1:
            CALLEXE(ePop);
        case 2:
            CALLEXE(eIn);
        case 3:
            CALLEXE(eDump);
        case 4:
            CALLEXE(eClear);
        case 5:
            CALLEXE(eAdd);
        case 6:
            CALLEXE(eSub);
        case 7:
            CALLEXE(eMul);
        case 8:
            CALLEXE(eDiv);
        case 9:
            CALLEXE(eSin);
        case 10:
            CALLEXE(eCos);
        case 11:
            CALLEXE(eSqrt);
        case 12:
            CALLEXE(ePow);
        case 13:
            CALLEXE(eHet);
        case 14:
            CALLEXE(eOut);
        default:
            return SPU_EXE_UNKNOWN;
    }
    return SPU_EXE_OK;
}
