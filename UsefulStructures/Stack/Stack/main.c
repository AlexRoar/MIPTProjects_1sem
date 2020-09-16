/**
  Basic stack
 */


#include <stdio.h>
#include <stdlib.h>

typedef struct{
    void* storage;
    size_t singleSize;
    size_t maxAllocation;
    size_t currentSize;
    
    void (*construct)(size_t initialSize, size_t singleElemSize);
    void (*push)(size_t initialSize, size_t singleElemSize);
} Stack;

int main(){
    
}
