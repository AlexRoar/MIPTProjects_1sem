/**
 * @file
 * @author Александр Дремов <dremov.me@gmail.com>
 * @version 1.0 *
 * @section LICENSE *
 * @copyright 2020 alexdremov. All rights reserved.
 */

// =============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#define PRECISION_DELTA 1e-5
#define INF_SQUARE_SOLUTIONS -1

/**
 * Used for pretty indices in unit test data
 */
enum Test_Indices
{
    A, B, C,
    X1, X2,
    NROOTS,
};
typedef enum Test_Indices Test_Indices;
 
/**
*  Square roots calculation
*
*  @param a [in] first coefficient
*  @param b [in] second coefficient
*  @param c [in] third coefficient
*  @param x1 [out] first root (passed by reference)
*  @param x2 [out] second root (passed by reference)
*  @return number of roots. INF_SQUARE_SOLUTIONS if infinite number of solutions
*/
int  squareRoots(double a, double b, double c, double* x1, double* x2);


/**
* Casts -0 to 0 with PRECISION_DELTA precision
* @param value  [in + out]   Value to cast (passed by reference)
*/
void normalizeNegativeZero(double* value);


/**
* Generates output for task solution
* @param solNumber [in] number of solutions
* @param x1  [in] first x
* @param x2  [in] second x
* @return exit code
*/
int  printProcessingOutput(int solNumber, double x1, double x2);


/**
* @param value [in] value to be checked
* @return if the value is almost zero with precision PRECISION_DELTA
*/
bool isAlmostZero(double value);


/**
* Performs checks for squareRoots function
* @param numberFailed - number of total tests failed
* @param firstFailed - first test failed
*/
bool unitTestsSquareSolve(int* numberFailed, int* firstFailed);


/**
 * Performs all unit tests for this scope
 * @param numberFailed - number of total tests failed
 * @param firstFailed - first test failed
*/
bool performAllUnits(int* numberFailed, int* firstFailed);


int main() {
    
    printf("Square equation solution\n"
           "(c) Aleksandr Dremov 2019\n");
    printf("Performing unit tests: ");
    
    int numberFailed = 0;
    int firstFailed = -1;
    
    if (performAllUnits(&numberFailed, &firstFailed)) {
        printf("success\n\n");
    } else {
        printf("failed! (%d failed,  №%d first failed)\n"
               "USAGE IS NOT SAFE\n\n", numberFailed, firstFailed);
    }
    
    printf("Enter 3 coefficients, separated by space: ");
    
    double a = 0, b = 0, c = 0;
    int num = scanf("%lg %lg %lg", &a, &b, &c);
    
    while (num != 3) {
        printf("Error receiving parameters, "
               "please try again\n");
        while (getchar() != '\n'){}
        printf("Enter 3 coefficients, separated by space: ");
        num = scanf("%lg %lg %lg", &a, &b, &c);
    }
    
    printf("Recieved %d arguments\n", num);
    
    double x1 = 0, x2 = 0;
    int solNumber = squareRoots(a, b, c, &x1, &x2);
    
    return printProcessingOutput(solNumber, x1, x2);
}


int squareRoots(double a, double b, double c, double* x1, double* x2) {
    
    assert(x1 != x2);
    assert(x1 != NULL);
    assert(x2 != NULL);
    
    if (isAlmostZero(a)) { // linear solution
        if (isAlmostZero(b)) {
            if (isAlmostZero(c))
                return INF_SQUARE_SOLUTIONS;
            else
                return 0;
        }
        *x1 = *x2 = -c / b;
        normalizeNegativeZero(x1);
        normalizeNegativeZero(x2);
        
        return 1;
    } else { // discriminant solution
        double dis = b * b - 4 * a * c;
        
        if (dis < 0)
            return 0;
        
        if (isAlmostZero(dis)) {
            *x1 = *x2 = (-b) / (2 * a);
            normalizeNegativeZero(x1);
            normalizeNegativeZero(x2);
            
            return 1;
        } else {
            *x1 = (-b - sqrt(dis)) / (2 * a);
            *x2 = (-b + sqrt(dis)) / (2 * a);
            normalizeNegativeZero(x1);
            normalizeNegativeZero(x2);
            
            return 2;
        }
    }
}


int printProcessingOutput(int solNumber, double x1, double x2){
    switch (solNumber) {
        case INF_SQUARE_SOLUTIONS:
            printf("Infinite solutions\n");
            break;
        case 0:
            printf("No solutions\n");
            break;
        case 1:
            printf("One solution: %lg\n", x1);
            break;
        case 2:
            printf("Two solutions: %lg and %lg\n", x1, x2);
            break;
        default:
            printf("main() ERROR: unhandled squareRoots() return value\n");
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


void normalizeNegativeZero(double* value){
    if (isAlmostZero(*value)) // handling "-0" case
        *value = 0;
}


bool isAlmostZero(double value){
    return fabs(value) <= PRECISION_DELTA;
}


bool unitTestsSquareSolve(int* numberFailed, int* firstFailed){
    double tests[][6] = { // tests in format {a, b, c, x1, x2, returnValue}
        {0, 0, 0,    0, 0, INF_SQUARE_SOLUTIONS},
        {0, 0, 1,    0, 0, 0},
        {1, -3, 2,   1, 2, 2},
        {1, 3, 2,   -2, -1, 2},
        {1, 2, 1,   -1, -1, 1},
        {1, 4, 4,   -2, -2, 1},
    };
    
    int numberOfTests = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; i < numberOfTests; i++) {
        double testX1 = 0, testX2 = 0;
        int retValue = 0;
        
        Test_Indices ind;
        
        retValue = squareRoots(
                               tests[i][ind = A],
                               tests[i][ind = B],
                               tests[i][ind = C],
                               &testX1,
                               &testX2
                               );
        if (retValue != tests[i][ind = NROOTS]) {
            (*numberFailed)++;
            if (*firstFailed == -1){
                *firstFailed = i + 1;
            }
            continue;
        }
        
        if (testX1 != tests[i][ind = X1] || testX2 != tests[i][ind = X2]) {
            (*numberFailed)++;
            if (*firstFailed == -1){
                *firstFailed = i + 1;
            }
            continue;
        }
        
    }
    return *numberFailed == 0;
}


bool performAllUnits(int* numberFailed, int* firstFailed) {
    
    assert(numberFailed != NULL);
    assert(firstFailed != NULL);
    
    *numberFailed = 0;
    *firstFailed = -1;
    
    unitTestsSquareSolve(numberFailed, firstFailed);
    
    return *numberFailed == 0;
}

