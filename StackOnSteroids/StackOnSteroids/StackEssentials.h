//
//  StackEssentials.h
//  StackOnSteroids
//
//  Created by Александр Дремов on 02.10.2020.
//

#ifndef StackEssentials_h
#define StackEssentials_h

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

#endif /* StackEssentials_h */
