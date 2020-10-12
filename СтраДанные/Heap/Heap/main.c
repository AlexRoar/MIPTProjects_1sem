#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned HeapElem;

// Heap structure
typedef struct {
    HeapElem* buffer;
    unsigned long long size;
    unsigned long long maxSize;
    int (*compareHeap)(HeapElem* a, HeapElem* b);
} Heap;


/**
 * Create new heap
 * @param maxSize Maximum alowed heap size
 */
Heap* NewHeap(unsigned long long maxSize, int (*compareHeap)(HeapElem* a, HeapElem* b)) {
    Heap* newHeap = calloc(1, sizeof(Heap));
    newHeap->buffer = calloc(maxSize, sizeof(HeapElem));
    newHeap->size = 0;
    newHeap->maxSize = maxSize;
    newHeap->compareHeap = compareHeap;
    return newHeap;
}

void DestructHeap(Heap* heap) {
    assert(heap);
    free(heap->buffer);
    free(heap);
}


static inline void swap(HeapElem* a, HeapElem* b) {
    assert(a);
    assert(b);
    HeapElem tmp = *a;
    *a = *b;
    *b = tmp;
}


static void siftDown(Heap* heap, unsigned long curIndex) {
    while (curIndex < heap->size) {
        unsigned long childR = (curIndex + 1) * 2;
        unsigned long childL = (curIndex + 1) * 2 - 1;
        if (childL >= heap->size)
            break;
        else{
            unsigned long newIndex = curIndex;
            if (childL < heap->size) {
                if (heap->compareHeap(heap->buffer + newIndex, heap->buffer + childL))
                    newIndex = childL;
            }
            if (childR < heap->size) {
                if (heap->compareHeap(heap->buffer + newIndex, heap->buffer + childR))
                    newIndex = childR;
            }
            if (newIndex == curIndex){
                break;
            }
            swap(&heap->buffer[curIndex], &heap->buffer[newIndex]);
            curIndex = newIndex;
        }
    }
}


static void siftUp(Heap* heap, unsigned long curIndex) {
    if (curIndex == 0)
        return;
    unsigned long parent = (curIndex - 1) / 2;
    while (heap->compareHeap(heap->buffer + parent, heap->buffer + curIndex)
           && curIndex != 0) {
        swap(&heap->buffer[parent], &heap->buffer[curIndex]);
        curIndex = parent;
        if (curIndex == 0)
            return;
        parent = (curIndex - 1) / 2;
    }
}


void pushHeap(Heap* heap, HeapElem elem) {
    assert(heap);
    unsigned long curIndex = heap->size;
    heap->buffer[curIndex] = elem;
    heap->size++;
    if (heap->size == 1)
        return;
    
    siftUp(heap, curIndex);
}


void popHeap(Heap* heap, HeapElem* elem){
    assert(heap);
    assert(elem);
    unsigned long curIndex = 0;
    *elem = heap->buffer[curIndex];
    heap->buffer[curIndex] = heap->buffer[heap->size - 1];
    siftDown(heap, curIndex);
    heap->size--;
}


void topHeap(Heap* heap, HeapElem* elem){
    *elem = heap->buffer[0];
}


//int validateHeap(Heap* h, int curIndex){
//    if (curIndex >= h->size)
//        return 1;
//    unsigned long long childR = (curIndex + 1) * 2;
//    unsigned long long childL = (curIndex + 1) * 2 - 1;
//
//    int result = 1;
//    if (childR <= h->size)
//        result &= ()
//}


int main() {

}
