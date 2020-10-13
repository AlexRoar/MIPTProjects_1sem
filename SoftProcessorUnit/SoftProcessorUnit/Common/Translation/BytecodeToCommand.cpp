//
//  BytecodeToCommand.cpp
//  SPUDisAsm
//
//  Created by Александр Дремов on 13.10.2020.
//

#include <string.h>
#include "BytecodeToCommand.hpp"
#include "DisAssemblyDTypes.hpp"
#include "TranslationDTypes.hpp"
#include "AssemblyHelpers.hpp"
#include "CommandsParser.hpp"
#include "Syntax.hpp"

int checkBytesEnough(const SyntaxEntity* thou, BinaryFile* binary, char* SPI) {
    size_t neededBytes = 1;
    
    neededBytes += thou->flagBytes;
    for (int i = 0; i < strlen(thou->format); i++) {
        if (thou->format[i] == '*')
            neededBytes++;
    }
    
    if (SPI + neededBytes >= binary->code + binary->currentSize)
        return 0;
    return 1;
}

OPBACKTRANSLATE_FUNC(bPush) {
    OPBACKGENERAL(
    char* localSPI = *SPI;
    char flagByte = *(localSPI + 1);
    if (flagByte == 0) {
        double value = *((double*)(localSPI + 2));
        fprintf(params->outputFile, "%lg", value);
        *SPI += sizeof(double) + 2;
    } else {
        const char* reg = registerNameFromNo(*(localSPI + 2));
        printf("%s", reg);
        *SPI += 2;
    }
    )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bPop) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bIn)  {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bDump) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bAdd) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bSub) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bMul) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bDiv) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bSin) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bCos) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bSqrt) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bPow) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bHet) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}

OPBACKTRANSLATE_FUNC(bOut) {
    OPBACKGENERAL(
                  (*SPI)++;
                  )
    return SPU_DISASM_OK;
}
