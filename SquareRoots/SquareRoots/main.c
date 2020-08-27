//
//  main.c
//  SquareRoots
//
//  Created by Александр Дремов on 27.08.2020.
//  Copyright © 2020 alexdremov. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <assert.h>

#define PRECISION_DELTA 1e-5
#define INF_SQUARE_SOLUTIONS -1


int squareRoots(double, double, double, double*, double*);
double squareDiscriminant(double, double, double);
void handleNegativeZero(double*);

int main(int argc, char *argv[]) {
    
    double a, b, c;
    a = b = c = 0;
    
    printf("Square equation solution\n(c) Aleksandr Dremov 2019\n\n");
    printf("Enter 3 coeeficients, separated by space: ");
    scanf("%lg %lg %lg", &a, &b, &c);
    
    double x1, x2;
    x1 = x2 = 0;
    
    int solNumber = squareRoots(a, b, c, &x1, &x2);
    
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
            return 1;
    }
    
    return 0;
}


/**
 *  Square roots calculation
 *
 *  @param a first coefficient
 *  @param b second coefficient
 *  @param c third coefficient
 *  @param x1 first root (passed by reference)
 *  @param x2 second root (passed by reference)
 *  @return number of roots. INF_SQUARE_SOLUTIONS if infinite number of solutions
 */
int squareRoots(double a, double b, double c, double* x1, double* x2) {
    
    assert(x1 != x2);
    assert(x1 != NULL);
    assert(x2 != NULL);
    
    if (fabs(a) <= PRECISION_DELTA) { // linear solution
        if (fabs(b) <= PRECISION_DELTA) {
            if (fabs(c) <= PRECISION_DELTA)
                return INF_SQUARE_SOLUTIONS;
            else
                return 0;
        }
        *x1 = *x2 = -c / b;
        handleNegativeZero(x1);
        handleNegativeZero(x2);
        
        return 1;
    } else { // discriminant solution
        double dis = squareDiscriminant(a, b, c);
        
        if (dis < 0)
            return 0;
        
        if (fabs(dis) <= PRECISION_DELTA) {
            *x1 = *x2 = (-b + sqrt(dis)) / (2 * a);
            handleNegativeZero(x1);
            handleNegativeZero(x2);
            
            return 1;
        } else {
            *x1 = (-b + sqrt(dis)) / (2 * a);
            *x2 = (-b - sqrt(dis)) / (2 * a);
            handleNegativeZero(x1);
            handleNegativeZero(x2);
            
            return 2;
        }
    }
}


/**
 *  Discriminant calculation
 *
 *  @param a first coefficient
 *  @param b second coefficient
 *  @param c third coefficient
 *  @return discriminant value
 */
double squareDiscriminant(double a, double b, double c) {
    return b * b - 4 * a * c;
}

/**
 * Casts -0 to 0 with PRECISION_DELTA precision
 * @param value  Value to cast (passed by reference)
 */
void handleNegativeZero(double* value){
    if (fabs(*value) <= PRECISION_DELTA) // handling "-0" case
        *value = 0;
}
