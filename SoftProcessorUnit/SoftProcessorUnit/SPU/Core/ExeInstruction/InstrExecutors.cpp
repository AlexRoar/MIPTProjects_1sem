//
//  InstrExecutors.cpp
//  SPU
//
//  Created by Александр Дремов on 14.10.2020.
//


#include "InstrExecutors.hpp"
#include <math.h>

#define STACKRESULT switch (result) {\
case STACK_OP_OK:\
    break;\
case STACK_OP_UNDERFLOW:\
    return SPU_EXE_STACK_UNDERFLOW;\
    break;\
case STACK_OP_OVERFLOW:\
    return SPU_EXE_STACK_OVERFLOW;\
    break;\
case STACK_OP_NOMEMORY:\
    return SPU_EXE_NOMEMORY;\
    break;\
case STACK_OP_INTEGRITYERR:\
    return SPU_EXE_CORRUPTED;\
    break;\
case STACK_OP_NULL:\
    return SPU_EXE_CORRUPTED;\
    break;\
case STACK_OP_PTRINVALID:\
    return SPU_EXE_CORRUPTED;\
    break;\
}

OPEXE_FUNC(ePush) {
    if (!HASBYTES(3))
        return SPU_EXE_NOARGS;
    char* localSPI = *SPI;
    char flagByte = *(localSPI + 1);
    if(flagByte == 0){
        if (!HASBYTES(sizeof(double) + 2))
            return SPU_EXE_NOARGS;
        double value = *((double*)(localSPI + 2));
        StackRigidOperationCodes result = StackPush(&(core->stack), value);
        
        STACKRESULT
        
        (*SPI) += sizeof(double) + 2;
    } else {
        char reg = *(localSPI + 2);
        
        StackRigidOperationCodes result = StackPush(&(core->stack), core->REG[reg]);
        
        STACKRESULT
        
        (*SPI) += 3;
    }
    return SPU_EXE_OK;
}


OPEXE_FUNC(ePop) {
    if (!HASBYTES(2))
        return SPU_EXE_NOARGS;
    char* localSPI = *SPI;
    char flagByte = *(localSPI + 1);
    if (flagByte == 0) {
        double tmp = 0;
        StackRigidOperationCodes result = StackPop(&(core->stack), &tmp);
        STACKRESULT
        (*SPI) += 2;
    } else {
        char reg = *(localSPI + 2);
        StackRigidOperationCodes result = StackPop(&(core->stack), core->REG + reg);
        STACKRESULT
        (*SPI) += 3;
    }
    return SPU_EXE_OK;
}


OPEXE_FUNC(eIn) {
    if (!HASBYTES(2))
        return SPU_EXE_NOARGS;
    char* localSPI = *SPI;
    char flagByte = *(localSPI + 1);
    
    double newVal = 0;
    printf("Requested value: ");
    int scanned = scanf("%lg", &newVal);
    if (scanned == 0){
        return SPU_EXE_INVALIDINPUT;
    }
    
    if (flagByte == 0) {
        StackRigidOperationCodes result = StackPush(&(core->stack), newVal);
        
        STACKRESULT

        (*SPI) += 2;
    } else {
        char reg = *(localSPI + 2);
        core->REG[reg] = newVal;
        (*SPI) += 3;
    }
    return SPU_EXE_OK;
}


OPEXE_FUNC(eDump) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    StackDump(core->stack, -1, params->inputFileName, "dump requested");
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eClear) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    StackRigidOperationCodes result = StackClear(core->stack);
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eAdd) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    double left = 0, right = 0;
    StackRigidOperationCodes result = StackPop(&(core->stack), &right);
    STACKRESULT
    result = StackPop(&(core->stack), &left);
    STACKRESULT
    
    result = StackPush(&(core->stack), left + right);
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eSub) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    double left = 0, right = 0;
    StackRigidOperationCodes result = StackPop(&(core->stack), &right);
    STACKRESULT
    result = StackPop(&(core->stack), &left);
    STACKRESULT
    
    result = StackPush(&(core->stack), left - right);
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eMul) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    double left = 0, right = 0;
    StackRigidOperationCodes result = StackPop(&(core->stack), &right);
    STACKRESULT
    result = StackPop(&(core->stack), &left);
    STACKRESULT
    
    result = StackPush(&(core->stack), left * right);
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eDiv) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    double left = 0, right = 0;
    StackRigidOperationCodes result = StackPop(&(core->stack), &right);
    STACKRESULT
    result = StackPop(&(core->stack), &left);
    STACKRESULT
    
    result = StackPush(&(core->stack), left / right);
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eSin) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    double left = 0;
    StackRigidOperationCodes result = StackPop(&(core->stack), &left);
    STACKRESULT
    
    result = StackPush(&(core->stack), sin(left));
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eCos) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    double left = 0;
    StackRigidOperationCodes result = StackPop(&(core->stack), &left);
    STACKRESULT
    
    result = StackPush(&(core->stack), cos(left));
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eSqrt) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    double left = 0;
    StackRigidOperationCodes result = StackPop(&(core->stack), &left);
    STACKRESULT
    
    result = StackPush(&(core->stack), sqrt(left));
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(ePow) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    double left = 0, right = 0;
    StackRigidOperationCodes result = StackPop(&(core->stack), &right);
    STACKRESULT
    result = StackPop(&(core->stack), &left);
    STACKRESULT
    
    result = StackPush(&(core->stack), pow(left, right));
    STACKRESULT
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eHet) {
    if (!HASBYTES(1))
        return SPU_EXE_NOARGS;
    core->terminated = 1;
    (*SPI)++;
    return SPU_EXE_OK;
}


OPEXE_FUNC(eOut) {
    if (!HASBYTES(2))
        return SPU_EXE_NOARGS;
    
    char* localSPI = *SPI;
    char flagByte = *(localSPI + 1);
    if(flagByte == 0){
        double value = 0;
        StackRigidOperationCodes result = StackBack(core->stack, &value);
        
        STACKRESULT
        
        printf("%lg\n", value);
        (*SPI) += 2;
    } else {
        if (!HASBYTES(3))
            return SPU_EXE_NOARGS;
        char reg = *(localSPI + 2);
        
        printf("%lg\n", core->REG[reg]);
        
        (*SPI) += 3;
    }
    return SPU_EXE_OK;
}


