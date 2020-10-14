//
//  BytecodeToCommand.hpp
//  SPUDisAsm
//
//  Created by Александр Дремов on 13.10.2020.
//

#ifndef BytecodeToCommand_hpp
#define BytecodeToCommand_hpp

#include <stdio.h>
#include "DisAssemblyDTypes.hpp"
#include "TranslationDTypes.hpp"
#include "AssemblyHelpers.hpp"
#include "CommandsParser.hpp"
#include "Syntax.hpp"


#define OPBACKTRANSLATE_FUNC(name) DisassemblyParseResult name (const SyntaxEntity* thou, DisassemblyParams* params, BinaryFile* binary, char** SPI)

#define OPBACKGENERAL(code) { \
fprintf(params->outputFile, "%s ", thou->naming);\
if (checkBytesEnough(thou, binary, *SPI) == 0) {return SPU_DISASM_NOTENOUGHARGS;}\
code \
fprintf(params->outputFile, "\n"); }

int checkBytesEnough(const SyntaxEntity* thou, BinaryFile* binary, char* SPI);

int checkBytesEnoughNumber(BinaryFile* binary, char* SPI, size_t bytes);

OPBACKTRANSLATE_FUNC(bPush) ;
OPBACKTRANSLATE_FUNC(bPop)  ;
OPBACKTRANSLATE_FUNC(bIn)   ;
OPBACKTRANSLATE_FUNC(bDump) ;
OPBACKTRANSLATE_FUNC(bClear);
 
OPBACKTRANSLATE_FUNC(bAdd)  ;
OPBACKTRANSLATE_FUNC(bSub)  ;
OPBACKTRANSLATE_FUNC(bMul)  ;
OPBACKTRANSLATE_FUNC(bDiv)  ;
 
OPBACKTRANSLATE_FUNC(bSin)  ;
OPBACKTRANSLATE_FUNC(bCos)  ;
OPBACKTRANSLATE_FUNC(bSqrt) ;
OPBACKTRANSLATE_FUNC(bPow)  ;
 
OPBACKTRANSLATE_FUNC(bHet)  ;
 
OPBACKTRANSLATE_FUNC(bOut)  ;


#endif /* BytecodeToCommand_hpp */
