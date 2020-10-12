//
//  Syntax.h
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 12.10.2020.
//

#ifndef Syntax_h
#define Syntax_h
#include <stdlib.h>

typedef struct {
    const char*  naming;
    const char   code;
    const char*  format;
    const char   flagBytes;
} SyntaxEntity;

typedef struct {
    const SyntaxEntity* entities;
    const size_t        number;
} SyntaxMapping;

SyntaxMapping getSyntaxMapping(void);

int   getSyntaxCode(const SyntaxMapping* mapping, const char* name);

const char* getSyntaxName(const SyntaxMapping* mapping,  const char code);

const SyntaxEntity* getSyntaxEntityByCode(const SyntaxMapping* mapping, const char code);

const SyntaxEntity* getSyntaxEntityByName(const SyntaxMapping* mapping, const char* name);

#endif /* Syntax_h */
