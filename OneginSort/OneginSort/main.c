//
//  main.c
//  OneginSort
//
//  Created by Александр Дремов on 28.08.2020.
//  Copyright © 2020 alexdremov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct SortedLinesContainer {
    char** sortedContainer;
    unsigned long size;
    unsigned long availableSize;
    int allocIncrement;
    unsigned long* sizes;
    
    void (*add)(struct SortedLinesContainer* this, char* line, bool fromEnd);
    void (*containerRealloc)(struct SortedLinesContainer* this);
    void (*cellRealloc)(struct SortedLinesContainer* this, unsigned long pos, unsigned long len);
    void (*shiftLeft)(struct SortedLinesContainer* this, unsigned long pos);
} SortedLinesContainer;


int strlexcmp(char* line1, char* line2, bool fromEnd);
void reverse(char* line, unsigned long len);
void shiftLeft(struct SortedLinesContainer* this, unsigned long pos);
void add(struct SortedLinesContainer* this, char* line, bool fromEnd);
void containerRealloc(struct SortedLinesContainer* this);
void cellRealloc(struct SortedLinesContainer* this, unsigned long pos, unsigned long len);

/**
 * True if line1 higher than line2
 */
int strlexcmp(char* line1, char* line2, bool fromEnd){
    unsigned long firstLen  = strlen(line1);
    unsigned long secondLen = strlen(line2);
    
    if (!fromEnd){
        return strcmp(line1, line2);
    } else {
        char tmpLine1[firstLen + 1], tmpLine2[secondLen + 1];
        strcpy(tmpLine1, line1);
        strcpy(tmpLine2, line2);
        reverse(tmpLine1, firstLen);
        reverse(tmpLine2, secondLen);
        return strcmp(tmpLine1, tmpLine2);
    }
    
    return true;
}


void reverse(char* line, unsigned long len) {
    if (len <= 1)
        return;
    
    char* begin = line;
    char* end = line + len - 1;
    char c;
    for (int i = 0; i < len / 2; i++){
        c = *end;
        *end = *begin;
        *begin = c;
        
        begin++;
        end--;
    }
}

void shiftLeft(struct SortedLinesContainer* this, unsigned long pos) {
    for (unsigned long i = this->size; i > pos; i--){
        unsigned long sourceLen = *(this->sizes + i - 1);
        if (*(this->sortedContainer + i) == NULL) {
            this->cellRealloc(this, i, sourceLen);
        } else {
            unsigned long distinLen = strlen(*(this->sortedContainer + i));
            if (distinLen < sourceLen){
                this->cellRealloc(this, i, sourceLen);
            }
        }
        strcpy(*(this->sortedContainer + i), *(this->sortedContainer + i - 1));
        *(this->sizes + i) = sourceLen;
    }
}

void add(struct SortedLinesContainer* this, char* line, bool fromEnd) {
    unsigned long len = strlen(line);
    if (this->size == 0){
        this->containerRealloc(this);
        this->cellRealloc(this, 0, len);
        strcpy(*(this->sortedContainer), line);
        *(this->sizes) = len;
        this->size += 1;
        return;
    }
    
    for (int i = 0; i < this->size; i++){
        int cmpresult = strlexcmp(*(this->sortedContainer + i), line, fromEnd);
        //        printf("%s comparison %s - %d(%d)\n", line, *(this->sortedContainer + i), i, cmpresult);
        if (cmpresult > 0) {
            //            printf("%s higher than %s - %d(%d)\n", line, *(this->sortedContainer + i), i, cmpresult);
            //            for(int i = 0; i < this->size + 1; i++){
            //                printf("\t1\t%d: %s\n", (i+1), *(this->sortedContainer + i));
            //            }
            this->containerRealloc(this);
            this->shiftLeft(this, i);
            //            for(int i = 0; i < this->size + 1; i++){
            //                printf("\t2\t%d: %s\n", (i+1), *(this->sortedContainer + i));
            //            }
            this->cellRealloc(this, i, len);
            strcpy(*(this->sortedContainer + i), line);
            *(this->sizes + i) = len;
            //            for(int i = 0; i < this->size + 2; i++){
            //                printf("\t3\t%d: %s\n", (i+1), *(this->sortedContainer + i));
            //            }
            this->size += 1;
            return;
        }
    }
    
    this->containerRealloc(this);
    this->cellRealloc(this, this->size, len);
    strcpy(*(this->sortedContainer + this->size), line);
    *(this->sizes + + this->size) = len;
    this->size += 1;
}


void containerRealloc(struct SortedLinesContainer* this) {
    if (this->sortedContainer == NULL){
        this->sortedContainer = calloc(this->allocIncrement, sizeof(char*));
        this->sizes = calloc(this->allocIncrement, sizeof(unsigned long));
        this->availableSize = this->allocIncrement;
        return;
    }
    if (this->size + 1 > this->availableSize){
        this->sortedContainer = realloc(this->sortedContainer,
                                        sizeof(char*) * (this->availableSize + this->allocIncrement));
        this->sizes = realloc(this->sizes,
                              sizeof(unsigned long) * (this->availableSize + this->allocIncrement));
        this->availableSize += this->allocIncrement;
    }
}

void cellRealloc(struct SortedLinesContainer* this, unsigned long pos, unsigned long len) {
    if (*(this->sortedContainer + pos) == NULL){
        *(this->sortedContainer + pos) = calloc(len + 1, sizeof(char));
    }else{
        *(this->sortedContainer + pos) = realloc(*(this->sortedContainer + pos) ,(len + 1) * sizeof(char));
    }
}


int main(int argc, const char * argv[]) {
    
    bool reversed = false;
    char* inputFileName = "input.txt";
    char* outputFileName = "output.txt";
    
    for(int i = 0; i < argc; i++){
        if (strcmp("-r", *(argv + i))){
            reversed = true;
            continue;
        }
        
        if (strcmp("-output", *(argv + i))){
            i++;
            strcpy(outputFileName, *(argv + i++));
            continue;
        }
        if (strcmp("-input", *(argv + i))){
            i++;
            strcpy(inputFileName, *(argv + i++));
            continue;
        }
    }
    
    SortedLinesContainer container;
    unsigned long sizes[] = {};
    
    container.size = 0;
    container.availableSize = 0;
    container.allocIncrement = 64;
    container.add = &add;
    container.containerRealloc = &containerRealloc;
    container.cellRealloc = &cellRealloc;
    container.shiftLeft = &shiftLeft;
    container.sizes = sizes;
    
    container.add(&container, "bbc", false);
    container.add(&container, "abcdf", false);
    container.add(&container, "accdee", false);
    container.add(&container, "accdeeee", false);
    
    for(int i = 0; i < container.size; i++){
        printf("%d: %s(%d)\n", (i+1), *(container.sortedContainer + i), *(container.sizes + i));
    }
}
