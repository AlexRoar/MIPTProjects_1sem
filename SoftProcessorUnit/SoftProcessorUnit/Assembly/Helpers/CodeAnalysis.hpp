//
//  CodeAnalysis.hpp
//  SPUAsm
//
//  Created by Александр Дремов on 15.10.2020.
//

#ifndef CodeAnalysis_hpp
#define CodeAnalysis_hpp

#include <stdio.h>
#include "Syntax.hpp"
#include "AssemblyDTypes.hpp"
#include "AssemblyHelpers.hpp"
#include "CommandsParser.hpp"
#include "StringDistance.hpp"

int generateErrors(SyntaxMapping* mapping, AssemblyParams* params, char* code);


int analyzeInstructionErrors(SyntaxMapping* mapping, AssemblyParams* params, char* line, int lineNo);


int analyzeInstructionWarnings(SyntaxMapping* mapping, AssemblyParams* params, char* line);

const SyntaxEntity* bestMatchCommand(SyntaxMapping* mapping, char* command);

#endif /* CodeAnalysis_hpp */
