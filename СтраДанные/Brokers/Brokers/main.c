//
//  main.c
//  Brokers
//
//  Created by Александр Дремов on 05.10.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HeapType unsigned long long

typedef struct {
    HeapType* buffer;
    size_t size;
    size_t maxSize;
} Heap;


Heap* newHeap(size_t maxSize){
    Heap* newHeap = calloc(1, sizeof(Heap));
    newHeap->buffer = calloc(maxSize + 1, sizeof(HeapType));
    newHeap->size = 0;
    newHeap->maxSize = maxSize;
    return newHeap;
}


void swap(HeapType* a, HeapType* b){
    HeapType tmp = *a;
    *a = *b;
    *b = tmp;
}


void pushHeap(Heap* heap, HeapType elem){
    heap->size++;
    size_t curIndex = heap->size;
    heap->buffer[curIndex] = elem;
    if (heap->size == 1)
        return;
    
    while (1){
        size_t parent = curIndex / 2;
        if (parent == 0)
            return;
        if (heap->buffer[parent] >= heap->buffer[curIndex] ){
            swap(&heap->buffer[parent], &heap->buffer[curIndex]);
            curIndex = parent;
        }else{
            return;
        }
    }
}


void popHeap(Heap* heap, HeapType* elem){
    size_t curIndex = 1;
    *elem = heap->buffer[curIndex];
    heap->buffer[curIndex] = heap->buffer[heap->size];
    while (curIndex <= heap->size){
        if (curIndex * 2 > heap->size)
            break;
        else{
            size_t minIndex = curIndex;
            if (curIndex * 2 <= heap->size){
                if (heap->buffer[curIndex * 2] < heap->buffer[curIndex])
                    minIndex = curIndex * 2;
            }
            if (curIndex * 2 + 1 <= heap->size){
                if (heap->buffer[curIndex * 2 + 1] < heap->buffer[minIndex])
                    minIndex = curIndex * 2 + 1;
            }
            if (minIndex == curIndex)
                break;
            swap(&heap->buffer[curIndex], &heap->buffer[minIndex]);
            curIndex = minIndex;
        }
    }
    heap->size--;
}

void printHeap(Heap* h){
    for(size_t i = 1; i < h->size + 1; i++){
        printf("%llu ", h->buffer[i]);
    }
    printf("\n");
}

size_t mergeHeap(Heap* heap){
    size_t summarize = 0;
    while (heap->size != 1) {
        HeapType elemPopped1 = 0, elemPopped2 = 0;
        popHeap(heap, &elemPopped1);
        popHeap(heap, &elemPopped2);
        pushHeap(heap, elemPopped1 + elemPopped2);
        summarize += (elemPopped1 + elemPopped2) * 100;
//        printHeap(heap);
    }
    return summarize;
}


int main() {
    size_t n = 0;
    scanf("%zd", &n);
    Heap* h = newHeap(n);
    
    for (size_t i = 0; i < n; i++){
        HeapType newElem = 0;
        scanf("%lld", &newElem);
        pushHeap(h, newElem);
//        printHeap(h);
    }

    printf("%.2f\n",(double)mergeHeap(h)/10000);
    
//    printHeap(h);
    
    free(h->buffer);
    free(h);
}
