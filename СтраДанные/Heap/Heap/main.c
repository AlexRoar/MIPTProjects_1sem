#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned HeapElem;

// Heap structure
typedef struct {
    HeapElem* buffer;
    unsigned long long size;
    unsigned long long maxSize;
    short int direction;
} Heap;

static inline int compareHeap(HeapElem* a, HeapElem* b);


/**
 * Create new heap
 * @param maxSize Maximum alowed heap size
 * @param direction > 0 => the biggest elem on the top, < 0 => the smallest elem on the top
 */
Heap* NewHeap(unsigned long long maxSize, short int direction) {
    Heap* newHeap = calloc(1, sizeof(Heap));
    newHeap->buffer = calloc(maxSize, sizeof(HeapElem));
    newHeap->size = 0;
    newHeap->maxSize = maxSize;
    newHeap->direction = direction;
    return newHeap;
}


/**
 * Destructs the heap
 * @param heap heap to be destructed
 */
void DestructHeap(Heap* heap) {
    assert(heap);
    free(heap->buffer);
    free(heap);
}


static void swap(HeapElem* a, HeapElem* b) {
    assert(a);
    assert(b);
    HeapElem tmp = *a;
    *a = *b;
    *b = tmp;
}


void siftDown(Heap* heap, unsigned long long curIndex){
    while (curIndex < heap->size) {
        unsigned long long childR = (curIndex + 1) * 2;
        unsigned long long childL = (curIndex + 1) * 2 - 1;
        if (childL >= heap->size)
            break;
        else{
            unsigned long long newIndex = curIndex;
            if (childL < heap->size){
                if ((compareHeap(heap->buffer + childL, heap->buffer + newIndex) < 0 && heap->direction < 0)||
                    (compareHeap(heap->buffer + childL, heap->buffer + newIndex) > 0 && heap->direction > 0))
                    newIndex = childL;
            }
            if (childR < heap->size){
                if ((compareHeap(heap->buffer + childR, heap->buffer + newIndex) < 0 && heap->direction < 0)||
                    (compareHeap(heap->buffer + childR, heap->buffer + newIndex) > 0 && heap->direction > 0))
                    newIndex = childR;
            }
            if (newIndex == curIndex)
                break;
            swap(&heap->buffer[curIndex], &heap->buffer[newIndex]);
            curIndex = newIndex;
        }
    }
}


void siftUp(Heap* heap, unsigned long long curIndex) {
    unsigned long long parent = (curIndex - 1) / 2;
    while (((compareHeap(heap->buffer + parent, heap->buffer + curIndex) > 0 && heap->direction < 0) ||
           (compareHeap(heap->buffer + parent, heap->buffer + curIndex) < 0 && heap->direction > 0))
           && curIndex != 0){
        swap(&heap->buffer[parent], &heap->buffer[curIndex]);
        curIndex = parent;
        parent = (curIndex - 1) / 2;
    }
}


/**
 * Push the new value to the heap
 * @param heap heap to be altered
 * @param elem new element
 */
void pushHeap(Heap* heap, HeapElem elem) {
    assert(heap);
    unsigned long long curIndex = heap->size;
    heap->buffer[curIndex] = elem;
    heap->size++;
    if (heap->size == 1)
        return;
    
    siftUp(heap, curIndex);
}


/**
 * Get value from the heap's top and rebuild it
 * @param heap heap to be altered
 * @param elem from the top
 */
void popHeap(Heap* heap, HeapElem* elem){
    assert(heap);
    assert(elem);
    unsigned long long curIndex = 0;
    *elem = heap->buffer[curIndex];
    heap->buffer[curIndex] = heap->buffer[heap->size - 1];
    siftDown(heap, curIndex);
    heap->size--;
}



void topHeap(Heap* heap, HeapElem* elem){
    *elem = heap->buffer[0];
}


void printHeapPyramid(Heap* h){
    unsigned long long s = 1;
    unsigned long long layer = 0;
    for(size_t i = 1; i <= h->size; i++){
        printf("%u ", h->buffer[i - 1]);
        layer++;
        if (layer == s){
            s *= 2;
            printf("\n");
            layer = 0;
        }
    }
    printf("\n");
}


void printHeap(Heap* h){
    for(size_t i = 0; i < h->size; i++){
        printf("%u ", h->buffer[i]);
    }
    printf("\n");
}

static inline int compareHeap(HeapElem* a, HeapElem* b){
    return *a - *b;
}


int main() {
    Heap* newHeap = NewHeap(100, 1);
    HeapElem elem = 0;
    
    pushHeap(newHeap, 10);
//    printHeap(newHeap);
    pushHeap(newHeap, 10);
//    printHeap(newHeap);
    pushHeap(newHeap, 20);
//    printHeap(newHeap);
    pushHeap(newHeap, 2);
    pushHeap(newHeap, 2);
    pushHeap(newHeap, 2);
    pushHeap(newHeap, 2);
    pushHeap(newHeap, 2);
    pushHeap(newHeap, 100);
    
    
    popHeap(newHeap, &elem);
    popHeap(newHeap, &elem);
    popHeap(newHeap, &elem);
    popHeap(newHeap, &elem);
    popHeap(newHeap, &elem);
    printHeapPyramid(newHeap);
    
    DestructHeap(newHeap);
}
