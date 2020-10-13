//
//  CommandToBytecode.hpp
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 13.10.2020.
//

#ifndef CommandToBytecode_hpp
#define CommandToBytecode_hpp

#include <stdio.h>
#include "Syntax.hpp"
#include "TranslationDTypes.hpp"
#include "AssemblyHelpers.hpp"


#define OPTRANSLATE_FUNC(name) CommandToBytesResult name (const struct SyntaxEntity* thou, AssemblyParams* compileParams, BinaryFile* binary, int argc, const char** argv)

#define DUMPCODEBLOCK for (;starting < ending; starting++) { fprintf(compileParams->lstFile, "%-3d ", (unsigned char)binary->code[starting]); }

#define DUMPCODEBLOCKHEX for (;starting < ending; starting++) { fprintf(compileParams->lstFile, "%2x ", (unsigned char)binary->code[starting]); }

#define LSTDUMPED(code) { \
    size_t starting = binary->currentSize; \
    { code } \
    \
    size_t ending = binary->currentSize;\
    fprintf(compileParams->lstFile, "%-5zu -> %5zu | ", starting + binary->codeOffset, ending + binary->codeOffset - 1);\
    fprintf(compileParams->lstFile, "%-2zu | ", ending - starting);\
    fprintf(compileParams->lstFile, "%-20s | ", argv[0]);\
    DUMPCODEBLOCKHEX\
    fprintf(compileParams->lstFile, "\n");\
}

OPTRANSLATE_FUNC(cPush);
OPTRANSLATE_FUNC(cPop) ;
OPTRANSLATE_FUNC(cIn)  ;
OPTRANSLATE_FUNC(cDump);

OPTRANSLATE_FUNC(cAdd) ;
OPTRANSLATE_FUNC(cSub) ;
OPTRANSLATE_FUNC(cMul) ;
OPTRANSLATE_FUNC(cDiv) ;

OPTRANSLATE_FUNC(cSin) ;
OPTRANSLATE_FUNC(cCos) ;
OPTRANSLATE_FUNC(cSqrt);
OPTRANSLATE_FUNC(cPow) ;

OPTRANSLATE_FUNC(cHet) ;

OPTRANSLATE_FUNC(cOut) ;

#endif /* CommandToBytecode_hpp */
