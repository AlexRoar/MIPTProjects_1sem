//
//  main.c
//  OneginSortv2
//
//  Created by Александр Дремов on 29.08.2020.
//  Copyright © 2020 alexdremov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


typedef struct string{
    char* contents;
    unsigned long len;
} string;


typedef struct SortedLinesContainer {
    /**
     * Pointer to the start of string containing substrings separated by \0
     * (\n replaced by \0)
     */
    char* fullBuffer;
    
    
    /**
     * Strings containing starts of substrings in fullBuffer
     */
    string* lines;
    
    
    /**
     * Whether to perform comparison from the end
     */
    bool fromEnd;
    
    
    /**
     * Number of objects in lines
     */
    unsigned long linesNumber;
    
    
    /**
     * Number of allocated elements of lines
     */
    unsigned long linesMaxNumber;
    
    
    /**
     * How much new elements will be allocated when running out of space
     */
    int allocIncrement;
    
    
    /**
     * Function used in qsort for regular comparison
     * @param line1 first  struct string object
     * @param line2 second struct string object
     */
    int (*compar)(const void* line1, const void* line2);
    
    
    /**
     * Function used in qsort for comparison from the end of the string
     * @param line1 first  struct string object
     * @param line2 second struct string object
     */
    int (*comparRev)(const void* line1, const void* line2);
    
    
    /**
     * Constructs default object
     * @param this object to be constructed
     * @param fullBuffer pointer to the first char of readed file
     * @param length length of the buffer
     * @param fromEnd whether to sort from the end
     */
    void (*construct)(struct SortedLinesContainer* this, char* fullBuffer, unsigned long length, bool fromEnd);
    
    
    /**
     * Performs sorting of lines array
     */
    void (*sort)(struct SortedLinesContainer* this);
    
    
    /**
     * Allocates memory so that lines object can contain one more element
     * Uses allocIncrement
     * @param this object to be constructed
     */
    void (*allocate)(struct SortedLinesContainer* this);
    
    
    /**
     * Frees space
     * @param this object to be constructed
     */
    void (*free)(struct SortedLinesContainer* this);
} SortedLinesContainer;


int  compar(const void *, const void*);
int  comparRev(const void *, const void*);
void sortContainer(struct SortedLinesContainer* this);
void allocateContainer(struct SortedLinesContainer* this);
void construct(struct SortedLinesContainer* this,
               char* fullBuffer, unsigned long length, bool fromEnd);
void constructFromFile(struct SortedLinesContainer* this, char* fileName, bool fromEnd);

/**
 * Frees memory allocated for container
 * @param this [in] container
 */
void freeContainer(struct SortedLinesContainer* this);


/**
 * More than 0 if line1 higher than line2 in lexicographic order
 * @param line1 [in] first line to compare
 * @param line2 [in] second line to compare
 * @param fromEnd [in] whether to compare from the end of the lines
 */
int strlexcmp(char* line1, char* line2, unsigned long len1, unsigned long len2, bool fromEnd);


/**
 * Reverses the line
 * @param line [in + out] pointer to the SortedLinesContainer
 * @param len [in] the length of the line
 */
void reverse(char* line, unsigned long len);


/**
 * Adds lines from the file to the container.
 * @param this [in+out] container
 * @param fileName [in] file name
 * @param reversed [in] reversed sort (from the end)
 * @return number of readed lines
 */
unsigned long addFileLinesToContainer(SortedLinesContainer* this, char* fileName, bool reversed);


/**
 * Outputs container to the file.
 * @param this [in] container
 * @param fileName [in] file name
 * @return number of lines written
 */
unsigned long outputContainer(SortedLinesContainer* this, char* fileName);


/**
 * Tests for SortedLinesContainer
 * @return success
 */
bool testContainer(void);


int main(int argc, const char * argv[]) {
    SortedLinesContainer container;
    container.construct = &construct;
    constructFromFile(&container, "input.txt", false);
    for (unsigned long i = 0; i < container.linesNumber; i++){
        printf("%s (%lu)\n", (container.lines + i)->contents, (container.lines + i)->len);
    }
    return 0;
}


void construct(struct SortedLinesContainer* this, char* fullBuffer, unsigned long length, bool fromEnd) {
    assert(this != NULL);
    assert(fullBuffer != NULL);
    
    this->allocIncrement = 1024;
    this->fullBuffer = calloc(length + 1, sizeof(char));
    this->linesNumber = 0;
    this->fromEnd = fromEnd;
//    this->linesMaxNumber = 0;
    this->allocate = &allocateContainer;
    this->compar = &compar;
    this->comparRev = &comparRev;
    this->sort = &sortContainer;
    this->free = &freeContainer;
    
    int curCounter = 0;
    for (unsigned long i = 0; i < length; i++){
        *(this->fullBuffer + i) = *(fullBuffer + i);
        if (curCounter == 0){
            this->allocate(this);
            string newLocated = {(this->fullBuffer + i), 0};
            *(this->lines + this->linesNumber) = newLocated;
            this->linesNumber += 1;
            curCounter++;
            continue;
        }
        if (*(this->fullBuffer + i) == '\n' || *(this->fullBuffer + i) == '\0') {
            *(this->fullBuffer + i) = '\0';
            (this->lines + this->linesNumber - 1)->len = curCounter;
            curCounter = 0;
            continue;
        }
        curCounter++;
    }
    if (curCounter != 0){
        (this->lines + this->linesNumber - 1)->len = curCounter;
    }
    *(this->fullBuffer + length) = '\0';
}


void freeContainer(struct SortedLinesContainer* this){
    if (this->lines){
        free(this->lines);
    }
    
    if (this->fullBuffer)
        free(this->fullBuffer);
}


void sortContainer(struct SortedLinesContainer* this) {
    if (!this->fromEnd) {
        qsort(this->lines, this->linesNumber, sizeof(string), this->compar);
    }else{
        qsort(this->lines, this->linesNumber, sizeof(string), this->comparRev);
    }
}


void allocateContainer(struct SortedLinesContainer* this) {
    assert(this != NULL);
    
    if (this->lines == NULL || this->linesMaxNumber == 0){
        this->lines = calloc(this->allocIncrement, sizeof(string*));
        
        if (this->lines == NULL){
            printf("Failed allocating memory in allocateContainer()\n");
            this->free(this);
            exit(EXIT_FAILURE);
        }
        this->linesMaxNumber = this->allocIncrement;
    } else {
        if (this->linesNumber + 1 >= this->linesMaxNumber){
            this->linesMaxNumber += this->allocIncrement;
            this->lines = realloc(this->lines, this->linesMaxNumber * sizeof(string*));
            if (this->lines == NULL){
                printf("Failed reallocating memory in allocateContainer()\n");
                this->free(this);
                exit(EXIT_FAILURE);
            }
            
        }
    }
}


int compar(const void* line1, const void* line2){
    assert(line1 != NULL);
    assert(line2 != NULL);
    assert(line1 != line2);
    
    string* line1String = (struct string *) line1;
    string* line2String = (struct string *) line2;
    
    return strcmp(line1String->contents, line2String->contents);
}


int comparRev(const void* line1, const void* line2){
    assert(line1 != NULL);
    assert(line2 != NULL);
    assert(line1 != line2);
    
    string* line1String = (struct string *) line1;
    string* line2String = (struct string *) line2;
    
    char* tmpLine1 = calloc(line1String->len + 1, sizeof(char));
    char* tmpLine2 = calloc(line2String->len + 1, sizeof(char));
    
    if (tmpLine1 == NULL || tmpLine2 == NULL){
        printf("Failed allocating memory in strlexcmp()\n");
        exit(EXIT_FAILURE);
    }
    
    strcpy(tmpLine1, line1String->contents);
    strcpy(tmpLine2, line2String->contents);
    reverse(tmpLine1, line1String->len);
    reverse(tmpLine2, line2String->len);
    
    int compareResult = strcmp(tmpLine1, tmpLine2);
    
    free(tmpLine1);
    free(tmpLine2);
    
    return compareResult;
}


void reverse(char* line, unsigned long len) {
    assert(line != NULL);
    
    if (len <= 1)
        return;
    
    char* begin = line;
    char* end = line + len - 1;
    char c;
    for (unsigned long i = 0; i < len / 2; i++){
        c = *end;
        *end = *begin;
        *begin = c;
        
        begin++;
        end--;
    }
}

void constructFromFile(struct SortedLinesContainer* this, char* fileName, bool fromEnd){
    char * buffer = 0;
    long length;
    FILE * fp = fopen (fileName, "r");
    
    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, length, fp);
        }else{
            printf("Can't allocate memory "
                   "for buffer in constructFromFile()\n");
            exit(EXIT_FAILURE);
        }
        fclose (fp);
        unsigned long lenElements = length / sizeof(char);
        this->lines = calloc(lenElements + 1, sizeof(string*));
        this->construct(this, buffer, lenElements, fromEnd);
        this->sort(this);
    } else {
        printf("Can't open %s for read\n", fileName);
        exit(EXIT_FAILURE);
    }
}
