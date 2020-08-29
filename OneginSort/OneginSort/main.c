/**
 * @file
 * @brief Created by Александр Дремов on 28.08.2020.
 * @copyright Copyright © 2020 alexdremov. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/**
 * Container to store strings in lexicographical order
 */
typedef struct SortedLinesContainer {
    /**
     * Array to pointers of strings, sorted in lexicographicl order
     */
    char** sortedContainer;
    
    
    /**
     * Current number of strings
     */
    unsigned long size;
    
    
    /**
     * Allocated memory for sortedContainer (number of elements)
     */
    unsigned long availableSize;
    
    
    /**
     * How much additional memory will be allocated when available size is not enough
     */
    int allocIncrement;
    
    
    /**
     * Sizes of strings in sortedContainer
     */
    unsigned long* sizes;
    
    
    void (*add)(struct SortedLinesContainer* this, char* line, bool fromEnd);
    void (*containerRealloc)(struct SortedLinesContainer* this);
    void (*cellRealloc)(struct SortedLinesContainer* this, unsigned long pos, unsigned long len);
    void (*shiftRight)(struct SortedLinesContainer* this, unsigned long pos);
} SortedLinesContainer;


/**
 * More than 0 if line1 higher than line2 in lexicographic order
 * @param line1 [in] first line to compare
 * @param line2 [in] second line to compare
 * @param fromEnd [in] whether to compare from the end of the lines
 */
int  strlexcmp(char* line1, char* line2, bool fromEnd);


/**
 * Reverses the line
 * @param line [in + out] pointer to the SortedLinesContainer
 * @param len [in] the length of the line
 */
void reverse(char* line, unsigned long len);


/**
 * Shifts all elements right till position pos
 * @param this  [in + out] pointer to the SortedLinesContainer
 * @param pos  [in] position defining the boundary of shift
 */
void shiftRight(struct SortedLinesContainer* this, unsigned long pos);


/**
 * Function that adds string to the container, taking order into account.
 * @param this [in + out] pointer to the SortedLinesContainer
 * @param line [in] line to be inserted
 * @param fromEnd [in] whether to compare lines from the end
 */
void add(struct SortedLinesContainer* this, char* line, bool fromEnd);


/**
 * Reallocate memory for sortedContainer and size so that it can save one more element. Uses allocIncrement.
 * @param this  [in + out] pointer to the SortedLinesContainer
 */
void containerRealloc(struct SortedLinesContainer* this);


/**
 * Realocate space for specific cell in sortedContainer so that it can contain new line.
 * @param this  [in + out] pointer to the SortedLinesContainer
 * @param pos [in] position in sortedContainer
 * @param len [in] needed length (without counting '\0')
 */
void cellRealloc(struct SortedLinesContainer* this, unsigned long pos, unsigned long len);


/**
 * Sets default values for the container
 * @param this  [out] pointer to the SortedLinesContainer
 */
void defaultContainer(struct SortedLinesContainer* this);


/**
 * Creates a line copy without punctuation marks.
 * @param lineIn [in] input line
 * @param lineOut [out] output line
 * @param inpLen [in] input line length
 */
void lineWithoutPunctuation(char* lineIn, char* lineOut, unsigned long inpLen);


int main(int argc, const char * argv[]) {
    
    bool reversed = false;
    char* inputFileName = "input.txt";
    char* outputFileName = "output.txt";
    
    for(int i = 0; i < argc; i++){
        if (strcmp("-r", *(argv + i)) == 0){
            reversed = true;
            continue;
        }
        
        if (strcmp("-output", *(argv + i)) == 0){
            i++;
            strcpy(outputFileName, *(argv + i++));
            continue;
        }
        if (strcmp("-input", *(argv + i)) == 0){
            i++;
            strcpy(inputFileName, *(argv + i++));
            continue;
        }
    }
    
    SortedLinesContainer container;
    defaultContainer(&container);
    
    FILE * fp;
    char * newLine = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(inputFileName, "r");
    
    if (fp == NULL){
        printf("Could not open %s file\n", inputFileName);
        return EXIT_FAILURE;
    }

    while ((read = getline(&newLine, &len, fp)) != -1) {
        if (strcmp(newLine, "\n") == 0 || strcmp(newLine, " ") == 0)
            continue;
        container.add(&container, newLine, reversed);
    }

    fclose(fp);
    if (newLine)
        free(newLine);
    
    fp = fopen(outputFileName, "w");
    if (fp == NULL){
        printf("Could not open %s file\n", outputFileName);
        return EXIT_FAILURE;
    }
    
    for(int i = 0; i < container.size; i++){
        fputs(*(container.sortedContainer + i), fp);
    }
    fclose(fp);
    
    return EXIT_SUCCESS;
}



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

void shiftRight(struct SortedLinesContainer* this, unsigned long pos) {
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
        char* noPunctLine1 = calloc(*(this->sizes + i) + 1, sizeof(char));
        char* noPunctLine2= calloc(len + 1, sizeof(char));
        lineWithoutPunctuation(*(this->sortedContainer + i), noPunctLine1, *(this->sizes + i));
        lineWithoutPunctuation(line, noPunctLine2, len);
        
        int cmpresult = strlexcmp(noPunctLine1, noPunctLine2, fromEnd);
        //        printf("%s comparison %s - %d(%d)\n", line, *(this->sortedContainer + i), i, cmpresult);
        if (cmpresult > 0) {
            //            printf("%s higher than %s - %d(%d)\n", line, *(this->sortedContainer + i), i, cmpresult);
            //            for(int i = 0; i < this->size + 1; i++){
            //                printf("\t1\t%d: %s\n", (i+1), *(this->sortedContainer + i));
            //            }
            this->containerRealloc(this);
            this->shiftRight(this, i);
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
//        printf("%d\n", *(this->sortedContainer + pos));
        *(this->sortedContainer + pos) = realloc(*(this->sortedContainer + pos) ,(len + 1) * sizeof(char));
    }
}


void defaultContainer(struct SortedLinesContainer* this) {
    this->size = 0;
    this->availableSize = 0;
    this->allocIncrement = 2048;
    this->add = &add;
    this->containerRealloc = &containerRealloc;
    this->cellRealloc = &cellRealloc;
    this->shiftRight = &shiftRight;
}

void lineWithoutPunctuation(char* lineIn, char* lineOut, unsigned long inpLen) {
    char* outPos = lineOut;
    for (int i = 0; i<inpLen;i++){
        if (ispunct(*(lineIn + i)) || *(lineIn + i) == ' ')
            continue;
        *(outPos++) = *(lineIn + i);
    }
}
