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
#include <assert.h>


/**
 * Container to store strings in lexicographical order
 */
typedef struct SortedLinesContainer {
    /**
     * Array to pointers of strings, sorted in lexicographical order
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
    unsigned long allocIncrement;
    
    
    /**
     * Sizes of strings in sortedContainer
     */
    unsigned long* sizes;
    
    
    void (*add)(struct SortedLinesContainer* this, char* line, bool fromEnd);
    void (*containerRealloc)(struct SortedLinesContainer* this);
    void (*cellRealloc)(struct SortedLinesContainer* this, unsigned long pos, unsigned long len);
    void (*shiftRight)(struct SortedLinesContainer* this, unsigned long pos);
    void (*free)(struct SortedLinesContainer* this);
} SortedLinesContainer;


/**
 * More than 0 if line1 higher than line2 in lexicographic order
 * @param line1 [in] first line to compare
 * @param line2 [in] second line to compare
 * @param fromEnd [in] whether to compare from the end of the lines
 */
int  strlexcmp(char* line1, char* line2, unsigned long len1, unsigned long len2, bool fromEnd);


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
 * @return output length
 */
unsigned long lineWithoutPunctuation(char* lineIn, char* lineOut, unsigned long inpLen);


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
 * Frees memory allocated for container
 * @param this [in] container
 */
void freeContainer(struct SortedLinesContainer* this);


/**
 * Tests for SortedLinesContainer
 * @return success
 */
bool testContainer(void);


/**
 * Main function of the program Sorts input file lexicographically and outputs to the output file.
 * Command line parameters can be passed:
 * @arg -r – sort from the end
 * @arg -input <filename> - name of the input file
 * @arg -output <filename> - name of the output file
 */
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
    printf("Aleksandr Dremov\n"
           "(c) 2020 all rights reserved\n\n");
    printf("Will sort %s file and output to %s\n", inputFileName, outputFileName);
    printf("Is sort from the end: %s\n", (reversed) ? "true" : "false");
    if (testContainer()){
        printf("Tests performed succesfully\n\n");
    } else {
        printf("Some tests failed. Review printed messages\n\n");
        exit(EXIT_FAILURE);
    }
    
    SortedLinesContainer container;
    defaultContainer(&container);
    
    unsigned long linesReaded = addFileLinesToContainer(&container, inputFileName, reversed);
    if (linesReaded != 0)
        printf("Readed %lu lines\n", linesReaded);
    else {
        printf("Error reading lines");
        container.free(&container);
        return EXIT_FAILURE;
    }
    unsigned long linesWritten = outputContainer(&container, outputFileName);
    
    if (linesWritten != 0)
        printf("Wrote %lu lines\n", linesWritten);
    else {
        printf("Error writing lines");
        container.free(&container);
        return EXIT_FAILURE;
    }
    container.free(&container);
    
    return EXIT_SUCCESS;
}


int strlexcmp(char* line1, char* line2, unsigned long len1, unsigned long len2, bool fromEnd){
    assert(line1 != NULL);
    assert(line2 != NULL);
    assert(line1 != line2);
    
    if (!fromEnd){
        return strcmp(line1, line2);
    } else {
        char* tmpLine1 = calloc(len1 + 1, sizeof(char));
        char* tmpLine2 = calloc(len2 + 1, sizeof(char));
        
        if (tmpLine1 == NULL || tmpLine2 == NULL){
            printf("Failed allocating memory in strlexcmp()\n");
            exit(EXIT_FAILURE);
        }
        
        strcpy(tmpLine1, line1);
        strcpy(tmpLine2, line2);
        reverse(tmpLine1, len1);
        reverse(tmpLine2, len2);
        int compareResult = strcmp(tmpLine1, tmpLine2);
        
        free(tmpLine1);
        free(tmpLine2);
        
        return compareResult;
    }
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


void shiftRight(struct SortedLinesContainer* this, unsigned long pos) {
    assert(this != NULL);
    
    for (unsigned long i = this->size; i > pos; i--){
        unsigned long sourceLen = *(this->sizes + i - 1);
        if (*(this->sortedContainer + i) == NULL) {
            this->cellRealloc(this, i, sourceLen);
        }
        *(this->sortedContainer + i) = *(this->sortedContainer + i - 1);
        *(this->sizes + i) = sourceLen;
    }
}


void add(struct SortedLinesContainer* this, char* line, bool fromEnd) {
    assert(this != NULL);
    assert(line != NULL);
    
    unsigned long len = strlen(line);
    if (this->size == 0){
        this->containerRealloc(this);
        this->cellRealloc(this, 0, len);
        strcpy(*(this->sortedContainer), line);
        *(this->sizes) = len;
        this->size += 1;
        return;
    }
    
    for (unsigned long i = 0; i < this->size; i++){
        char* noPunctLine1 = calloc(*(this->sizes + i) + 1, sizeof(char));
        char* noPunctLine2 = calloc(len + 1, sizeof(char));
        
        if (noPunctLine1 == NULL || noPunctLine2 == NULL){
            printf("Failed allocating memory in add()\n");
            exit(EXIT_FAILURE);
        }
        
        unsigned long lenOut1 = lineWithoutPunctuation(*(this->sortedContainer + i), noPunctLine1, *(this->sizes + i));
        unsigned long lenOut2 = lineWithoutPunctuation(line, noPunctLine2, len);
        
        int cmpresult = strlexcmp(noPunctLine1, noPunctLine2, lenOut1, lenOut2, fromEnd);
        free(noPunctLine1);
        free(noPunctLine2);

        if (cmpresult > 0) {
            this->containerRealloc(this);
            this->shiftRight(this, i);

            *(this->sortedContainer + i) = NULL;
            this->cellRealloc(this, i, len);
            
            strcpy(*(this->sortedContainer + i), line);
            *(this->sizes + i) = len;

            this->size += 1;
            return;
        }
    }
    
    this->containerRealloc(this);
    this->cellRealloc(this, this->size, len);
    strcpy(*(this->sortedContainer + this->size), line);
    *(this->sizes + this->size) = len;
    this->size += 1;
}


void containerRealloc(struct SortedLinesContainer* this) {
    assert(this != NULL);
    
    if (this->sortedContainer == NULL || this->availableSize == 0){
        this->sortedContainer = calloc(this->allocIncrement, sizeof(char*));
        if (this->sortedContainer == NULL){
            printf("Failed allocating memory for sortedContainer in containerRealloc()\n");
            exit(EXIT_FAILURE);
        }
        this->sizes = calloc(this->allocIncrement, sizeof(unsigned long));
        if (this->sizes == NULL){
            printf("Failed allocating memory for sizes in containerRealloc()\n");
            exit(EXIT_FAILURE);
        }
        this->availableSize = this->allocIncrement;
        return;
    }
    if (this->size + 1 >= this->availableSize){
        this->sortedContainer = realloc(this->sortedContainer,
                                        sizeof(char*) * (this->availableSize + this->allocIncrement));
        
        if (this->sortedContainer == NULL){
            printf("Failed reallocating memory for sortedContainer\n");
            exit(EXIT_FAILURE);
        }
        this->sizes = realloc(this->sizes,
                              sizeof(unsigned long) * (this->availableSize + this->allocIncrement));
        if (this->sizes == NULL){
            printf("Failed reallocating memory for sizes\n");
            exit(EXIT_FAILURE);
        }
        this->availableSize += this->allocIncrement;
    }
}


void cellRealloc(struct SortedLinesContainer* this, unsigned long pos, unsigned long len) {
    assert(this != NULL);
    
    if (*(this->sortedContainer + pos) == NULL || *(this->sizes + pos) == 0){
        *(this->sortedContainer + pos) = calloc(len + 1, sizeof(char));
        if (*(this->sortedContainer + pos) == NULL){
            printf("Failed reallocating memory for sortedContainer cell in cellRealoc()\n");
            exit(EXIT_FAILURE);
        }
    }else{
        *(this->sortedContainer + pos) = realloc(*(this->sortedContainer + pos), (len + 1) * sizeof(char));
    }
    if (this->sortedContainer == NULL){
        printf("Failed reallocating memory for sortedContainer cell\n");
        exit(EXIT_FAILURE);
    }
}


void defaultContainer(struct SortedLinesContainer* this) {
    assert(this != NULL);
    this->size = 0;
    this->availableSize = 0;
    this->allocIncrement = 2048;
    this->add = &add;
    this->containerRealloc = &containerRealloc;
    this->cellRealloc = &cellRealloc;
    this->shiftRight = &shiftRight;
    this->free = &freeContainer;
    this->sizes = NULL;
}


unsigned long lineWithoutPunctuation(char* lineIn, char* lineOut, unsigned long inpLen) {
    assert(lineIn != NULL);
    assert(lineOut != NULL);
    assert(lineOut != lineIn);
    
    char* outPos = lineOut;
    for (unsigned long i = 0; i<inpLen;i++){
        if (ispunct(*(lineIn + i)) || *(lineIn + i) == ' ')
            continue;
        *(outPos++) = *(lineIn + i);
    }
    return (unsigned long)(outPos - lineOut);
}


unsigned long addFileLinesToContainer(SortedLinesContainer* this, char* fileName, bool reversed) {
    assert(this != NULL);
    
    FILE* fp;
    char* newLine = NULL;
    size_t len = 0;
    unsigned long linesReaded = 0;

    fp = fopen(fileName, "r");
    
    if (fp == NULL){
        printf("Could not open %s file\n", fileName);
        return 0;
    }

    while ((getline(&newLine, &len, fp)) != -1) {
        if (!(strcmp(newLine, "\n") == 0 || strcmp(newLine, " ") == 0)){
            this->add(this, newLine, reversed);
            linesReaded++;
        }
    }
    if (newLine)
        free(newLine);

    fclose(fp);
    
    return linesReaded;
}


unsigned long outputContainer(SortedLinesContainer* this, char* fileName) {
    assert(this != NULL);
    
    FILE* fp;
    
    fp = fopen(fileName, "w");
    if (fp == NULL){
        printf("Could not open %s file\n", fileName);
        return 0;
    }
    
    for(unsigned long i = 0; i < this->size; i++){
        fputs(*(this->sortedContainer + i), fp);
    }
    
    fclose(fp);
    return this->size;
}


void freeContainer(struct SortedLinesContainer* this) {
    assert(this != NULL);
    
    for (unsigned long i = 0; i < this->availableSize; i++) {
        if (*(this->sortedContainer) != NULL) {
            free(*(this->sortedContainer));
            *(this->sortedContainer) = NULL;
        }
    }
    if (this->sortedContainer != NULL)
        free(this->sortedContainer);
    if (this->sizes != NULL)
        free(this->sizes);
    this->sortedContainer = NULL;
}

bool testContainer(void){
    int failedNum = 0;
    int number = 1;
    
    SortedLinesContainer container;
    defaultContainer(&container);
    
    container.add(&container, "abcd", false);
    if (!(container.size == 1 && strcmp(*(container.sortedContainer), "abcd") == 0)){
        failedNum++;
        printf("Failed testContainer test %d \n", number);
    }
    freeContainer(&container);
    number++;
    
    defaultContainer(&container);
    container.add(&container, "abcd", true);
    if (!(container.size == 1 && strcmp(*(container.sortedContainer), "abcd") == 0)){
        failedNum++;
        printf("Failed testContainer test %d \n", number);
    }
    freeContainer(&container);
    number++;
    
    defaultContainer(&container);
    container.add(&container, "abcd", false);
    container.add(&container, "accd", false);
    if (!(container.size == 2 && strcmp(*(container.sortedContainer), "abcd") == 0 && strcmp(*(container.sortedContainer + 1), "accd") == 0)){
        failedNum++;
        printf("Failed testContainer test %d \n", number);
    }
    freeContainer(&container);
    number++;
    
    defaultContainer(&container);
    container.add(&container, "abca", true);
    container.add(&container, "accb", true);
    if (!(container.size == 2 && strcmp(*(container.sortedContainer), "abca") == 0 && strcmp(*(container.sortedContainer + 1), "accb") == 0)){
        failedNum++;
        printf("Failed testContainer test %d \n", number);
    }
    freeContainer(&container);
    number++;
    
    defaultContainer(&container);
    container.add(&container, "abca", false);
    container.add(&container, "accb", false);
    container.add(&container, "aaaaaaaaa", false);
    if (!(container.size == 3 &&
          strcmp(*(container.sortedContainer), "aaaaaaaaa") == 0 &&
          strcmp(*(container.sortedContainer + 1), "abca") == 0 &&
          strcmp(*(container.sortedContainer + 2), "accb") == 0)){
        failedNum++;
        printf("Failed testContainer test %d \n", number);
    }
    freeContainer(&container);
    number++;
    
    defaultContainer(&container);
    if (!(container.size == 0)){
        failedNum++;
        printf("Failed testContainer test %d \n", number);
    }
    freeContainer(&container);
    number++;
    
    return failedNum == 0;
}
