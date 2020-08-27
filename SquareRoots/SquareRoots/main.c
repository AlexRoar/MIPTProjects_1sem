//
//  main.c
//  SquareRoots
//
//  Created by Александр Дремов on 27.08.2020.
//  Copyright © 2020 alexdremov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#define PRECISION_DELTA 1e-5
#define INF_SQUARE_SOLUTIONS -1


int  squareRoots(double a, double b, double c, double* x1, double* x2);
void normalizeNegativeZero(double* value);
int  printProcessingOutput(int solNumber, double x1, double x2);
bool isAlmostZero(double value);


int main() {
    
    printf("Square equation solution\n"
           "(c) Aleksandr Dremov 2019\n\n");
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
            *x1 = (-b + sqrt(dis)) / (2 * a);
            *x2 = (-b - sqrt(dis)) / (2 * a);
            normalizeNegativeZero(x1);
            normalizeNegativeZero(x2);
            
            return 2;
        }
    }
}


/**
 * Generates output for task solution
 * @param solNumber [in] number of solutions
 * @param x1  [in] first x
 * @param x2  [in] second x
 * @return exit code
 */
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


/**
 * Casts -0 to 0 with PRECISION_DELTA precision
 * @param value  [in + out]   Value to cast (passed by reference)
 */
void normalizeNegativeZero(double* value){
    if (isAlmostZero(*value)) // handling "-0" case
        *value = 0;
}


/**
 * @param value [in] value to be checked
 * @return if the value is almost zero with precision PRECISION_DELTA
 */
bool isAlmostZero(double value){
    return fabs(value) <= PRECISION_DELTA;
}
