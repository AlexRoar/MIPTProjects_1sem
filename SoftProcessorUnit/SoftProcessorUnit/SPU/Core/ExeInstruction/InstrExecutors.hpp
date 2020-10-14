//
//  InstrExecutors.hpp
//  SPU
//
//  Created by Александр Дремов on 14.10.2020.
//

#ifndef InstrExecutors_hpp
#define InstrExecutors_hpp

#include <stdio.h>

#include "InstrExecutors.hpp"
#include "SPUDtypes.hpp"
#include "SPUCore.hpp"
#include "SPUCoreHelpers.hpp"

#define OPEXE_FUNC(name) InstructionExeResult name (SPUCore* core, RunParameters* params, BinaryFile* binary, char** SPI)

#define HASBYTES(bytes) ( ( bytes ) + *SPI <= binary->currentSize + binary->code)

OPEXE_FUNC(ePush) ;
OPEXE_FUNC(ePop)  ;
OPEXE_FUNC(eIn)   ;
OPEXE_FUNC(eDump) ;
OPEXE_FUNC(eClear);

OPEXE_FUNC(eAdd)  ;
OPEXE_FUNC(eSub)  ;
OPEXE_FUNC(eMul)  ;
OPEXE_FUNC(eDiv)  ;

OPEXE_FUNC(eSin)  ;
OPEXE_FUNC(eCos)  ;
OPEXE_FUNC(eSqrt) ;
OPEXE_FUNC(ePow)  ;

OPEXE_FUNC(eHet)  ;

OPEXE_FUNC(eOut)  ;

#endif /* InstrExecutors_hpp */
