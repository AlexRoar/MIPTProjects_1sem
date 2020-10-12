//
//  main.c
//  QSort
//
//  Created by Александр Дремов on 10.10.2020.
//

#include <stdio.h>
#include <stdlib.h>

void inline static swap(void* a, void* b, size_t size);

void qsortMy(void* base, size_t n_el, size_t width, int (*cmp)(const void* a, const void* b));

void qsortPartition(void* lo, void* hi, size_t width, int (*cmp)(const void* a, const void* b));

int main(int argc, const char * argv[]) {
    // insert code here...
    
    printf("Hello, World!\n");
    return 0;
}

void inline static swap(void* a, void* b, size_t size){
    size_t __size = (size);
    char* __a = (char*)(a), *__b = (char*)(b);
    do
    {
        char __tmp = *__a;
        *__a++ = *__b;
        *__b++ = __tmp;
    } while (--__size > 0);
}

void qsortMy(void* base, size_t n_el, size_t width, int (*cmp)(const void* a, const void* b)) {
    if (n_el == 0 || width == 0)
        return;
    char *base_ptr = (char *) base;
    
    
}


void qsortPartition(void* lo, void* hi, size_t width, int (*cmp)(const void* a, const void* b)) {
    
}
