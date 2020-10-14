//
//  CommandToBytecode.cpp
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 13.10.2020.
//

#include "CommandToBytecode.hpp"
#include "AssemblyHelpers.hpp"
#include "CommandsParser.hpp"
#include "Syntax.hpp"
#include <string.h>
#include <ctype.h>


OPTRANSLATE_FUNC(cPush) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
        const char* firstArgument = argv[1];
        if (isdigit(*firstArgument) || *firstArgument == '-') {     // flag is 1 if there's register
            appendToBinFile(binary, (char)0);
            double number = 0;
            int parsed = sscanf(firstArgument, "%lg", &number);
            if (parsed == 0)
                return SPU_CTB_INVALID_NUMBER;
            appendToBinFile(binary, (void*)&number, sizeof(number));
        } else {
            appendToBinFile(binary, (char)1);
            int registerNo = registerNoFromName((char*)firstArgument);
            if (registerNo == -1)
                return SPU_CTB_UNKNOWN_REGISTER;
            char registerNoCh = (char)registerNo;
            appendToBinFile(binary, (void*)&registerNoCh, sizeof(registerNoCh));
        }
    })
    return SPU_CTB_OK;
}
OPTRANSLATE_FUNC(cPop){
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
        
        if (argc == 1) {
            appendToBinFile(binary, (char)0);
        } else {
            const char* firstArgument = argv[1];
            appendToBinFile(binary, (char)1);
            
            int registerNo = registerNoFromName((char*)firstArgument);
            if (registerNo == -1)
                return SPU_CTB_UNKNOWN_REGISTER;
            
            char registerNoCh = (char)registerNo;
            appendToBinFile(binary, (void*)&registerNoCh, sizeof(registerNoCh));
        }
    })
    return SPU_CTB_OK;
}
OPTRANSLATE_FUNC(cIn){
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
        
        if (argc == 1) {
            appendToBinFile(binary, (char)0);
        } else {
            const char* firstArgument = argv[1];
            appendToBinFile(binary, (char)1);
            
            int registerNo = registerNoFromName((char*)firstArgument);
            if (registerNo == -1)
                return SPU_CTB_UNKNOWN_REGISTER;
            
            char registerNoCh = (char)registerNo;
            appendToBinFile(binary, (void*)&registerNoCh, sizeof(registerNoCh));
        }
    })
    return SPU_CTB_OK;
}

OPTRANSLATE_FUNC(cClear) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}

OPTRANSLATE_FUNC(cDump) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}

OPTRANSLATE_FUNC(cAdd) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}
OPTRANSLATE_FUNC(cSub) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}
OPTRANSLATE_FUNC(cMul) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}
OPTRANSLATE_FUNC(cDiv) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}

OPTRANSLATE_FUNC(cSin) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}
OPTRANSLATE_FUNC(cCos) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}
OPTRANSLATE_FUNC(cSqrt) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}
OPTRANSLATE_FUNC(cPow) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}

OPTRANSLATE_FUNC(cHet) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
    })
    return SPU_CTB_OK;
}

OPTRANSLATE_FUNC(cOut) {
    LSTDUMPED({
        appendToBinFile(binary, (void*)&thou->code, 1);
        if (argc > 1){
            const char* firstArgument = argv[1];
            appendToBinFile(binary, (char)1);
            
            int registerNo = registerNoFromName((char*)firstArgument);
            if (registerNo == -1)
                return SPU_CTB_UNKNOWN_REGISTER;
            
            char registerNoCh = (char)registerNo;
            appendToBinFile(binary, (void*)&registerNoCh, sizeof(registerNoCh));
        }else{
            appendToBinFile(binary, (char)0);
        }
    })
    return SPU_CTB_OK;
}
