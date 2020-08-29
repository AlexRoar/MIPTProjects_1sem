//
//  main.c
//  OneginSortv2
//
//  Created by Александр Дремов on 29.08.2020.
//  Copyright © 2020 alexdremov. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>

typedef struct SortedLinesContainer {
    char* fullBuffer;
    char** lines;
    int (*compar)(const void *, const void*);
    int (*comparRev)(const void *, const void*);
    bool fromEnd;
    unsigned long linesNumber;
    unsigned long* linesSizes;
    void (*construct)(struct SortedLinesContainer* this, char* fullBuffer);
    void (*sort)(struct SortedLinesContainer* this);
} SortedLinesContainer;

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
