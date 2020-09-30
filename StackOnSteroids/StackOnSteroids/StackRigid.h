/**
 * @mainpage
 * @author Aleksandr Dremov
 * @brief This Stack is almost impossible to kill and fool. All undefined behaviour memory accesses will be spoted and reported.
 * @copyright Aleksandr Dremov, MIPT 2020
 */

#ifndef StackRigid_h
#define StackRigid_h

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>


/**
 * Element type to be used in Stack
 */
typedef int StackElementType;

/**
 * The main structure. There's no definition in the header file in order to block direct parameters access
 */
typedef struct StackRigid StackRigid;


/**
 * Codes returned after operations on Stack
 */
typedef enum StackRigidOperationCodes{
    /**
     * Succesful operation
     */
    STACK_OP_OK,
    /**
     * Tried to get value from the empty stack
     */
    STACK_OP_UNDERFLOW,
    /**
     * Stack overflow and new memory allocation failed
     */
    STACK_OP_OVERFLOW,
    /**
     * No memory for stack alocations
     */
    STACK_OP_NOMEMORY,
    /**
     * Stack memory was altered by some outer access
     */
    STACK_OP_INTEGRITYERR,
    /**
     * Found NULL pointer during the operation
     */
    STACK_OP_NULL
} StackRigidOperationCodes;


/**
 * Codes characterizing Stack state
 */
typedef enum StackRigidState{
    /**
     * Stack is running as expected
     */
    STACK_ST_OK,
    /**
     * Stack was altered by some outer access
     */
    STACK_ST_INTEGRITYERR,
    /**
     * Stack is uninitialized
     */
    STACK_ST_UNINITIALIZED,
    /**
     * Found NULL pointer during the operation
     */
    STACK_ST_NULL
} StackRigidState;


/**
 * Create new stack with pre-defined capacity
 * @param[in] capacity initial stack capacity. Set to 0 if yo want the stack to adopt automaticaly.
 * @param[in,out] logFile file for logging
 * @return new stack pointer
 */
StackRigid* NewStackRigid(const size_t capacity, FILE* logFile);


/**
 * Perform all stack checks: checksums, general constrains.
 * @param[in] stack Stack to be validated
 * @return calculated state
 */
StackRigidState StackValidate(StackRigid* stack);


/**
 * Push the value to the stack
 * The operation can perform realocations if there is not enough space
 * @param[in,out] stack Stack to be changed
 * @param[in] value value to be pushed
 * @return the outcome of the operation
 */
StackRigidOperationCodes StackPush(StackRigid** stack, StackElementType value);


/**
 * Pop value from the end of the stack
 * The operation can perform realocations if there is too much space
 * @param[in,out] stack Stack to be distructed
 * @param[out] value value to be pushed
 * @return the outcome of the operation
 */
StackRigidOperationCodes StackPop(StackRigid** stack, StackElementType* value);


/**
 * Free stack memory
 * @param[in,out] stack Stack to be distructed
 * @return the outcome of the operation
 */
StackRigidOperationCodes StackDestruct(StackRigid** stack);


/**
 * Free stack memory
 * @param[out] stack Stack to be used
 * @return the outcome of the operation
 */
StackRigidOperationCodes StackBack(StackRigid* stack, StackElementType* value);


/**
 * Dumps debug information about the stack.
 * @param[in] stack Stack to be dumped
 */
void StackDump(StackRigid* stack);


/**
 * Memory alocated for the Stack
 * @param[in] stack Stack to be checked
 * @return alocated bytes
 */
size_t StackRigidMemoryUse(StackRigid* stack);


/**
 * @attention Not for the manual use!
 * @brief Updates Stack checksums. Does not take into account bytes inside the stack but outside of its last element.
 * @verbatim
 * Stack memory :  [ __stack area__ | value 1, value 2, ..., value n || ___garbage area___]
 * ____________________________________________________________________^ alterations in this area are not checked
 * @endverbatim
 * @param[in,out] stack Stack that checksums are needed to be updated
 */
static void __StackUpdateChecksum(StackRigid* stack);


/**
 * @attention Not for the manual use!
 * @brief Claculates big Stack checksum. Uses Adler-32 method
 * @verbatim
 * Stack memory : [ checkSum checkSumVital capacity size logFile | value 1, value 2, ..., value n || ___garbage area___]
 * ________________from here ^__________________________________________________________^to here
 * @endverbatim
 * @param[in] stack Stack that checksums are needed to be updated
 */
static uint32_t __StackGetChecksum(StackRigid* stack);


/**
 * @attention Not for the manual use!
 * @brief Uses Adler-32 method. Calculates Stack checksum only for the vital parameters.
 * This is necessary because __StackGetChecksum() relies on capacity and size. If they were changed, behaviour is undefined
 *
 * @verbatim
 * Stack memory : [ checkSum checkSumVital capacity size logFile | value 1, value 2, ..., value n || ___garbage area___]
 * ______________________________from here ^__________________^to here
 * @endverbatim
 * @param[in] stack Stack that checksums are needed to be updated
 */
static uint32_t __StackGetChecksumVital(StackRigid* stack);


/**
 * @attention Not for the manual use!
 * @brief Realocates Stack if needed
 * @param[in,out] stack Stack to be realocated
 * @param[in] direction which direction realocations are available.  > 0 -> expand space if needed, < 0 -> shrink space if needed
 */
static StackRigidOperationCodes __StackRealocate(StackRigid** stack, short int direction);


/**
 * @attention Not for the manual use!
 * @brief Calculates Adler-32 checksum from the firstBlock to the firstBlock + len
 * @param[in] firstBlock first block to include to the checksum
 * @param[in] len length of byte sequence
 * @return checksum
 */
static uint32_t adlerChecksum(const void* firstBlock, size_t len);

#endif /* StackRigid_h */
