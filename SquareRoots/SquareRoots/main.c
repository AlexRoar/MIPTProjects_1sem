//
//  main.c
//  SquareRoots
//
//  Created by Александр Дремов on 27.08.2020.
//  Copyright © 2020 alexdremov. All rights reserved.
//

#include <stdio.h>
#include <math.h>

#define PRECISION_DELTA 1e-5

int squareRoots(float, float, float, float*, float*);

int main(int argc, char *argv[]) {
    
    return 0;
}

int squareRoots(float a, float b, float c, float* x1, float* x2) {
    if (fabsf(a) <= PRECISION_DELTA) { // linear solution
        *x1 = *x2 = -c / b;
        return 1;
    } else {
        
    }
    return 1;
}

float squareDiscriminant(float a, float b, float c) {
    return b * b - 4 * a * c;
}
