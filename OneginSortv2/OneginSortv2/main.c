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
 * Used to store char* and its len so that computations are more effective
 */
typedef struct string {
    
    
    char* contents;
    
    
    unsigned long len;
    
    
    /**
     * Whether contents were alocated by malloc (can be freed)
     */
    bool allocated;
} string;


typedef struct SortedLinesContainer {
    /**
     * Pointer to the start of the string containing substrings separated by \0
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
     * Number of max elements in lines
     */
    unsigned long linesMaxNumber;
    
    
    /**
     * How much new elements will be allocated when running out of space
     */
    unsigned long allocIncrement;
    
    
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
     * Invisible charecters are trimmed and blank lines are removed
     * @param this object to be constructed
     * @param fullBuffer pointer to the first char of readed file
     * @param length length of the buffer
     * @param fromEnd whether to sort from the end
     */
    int (*construct)(struct SortedLinesContainer* this, char* fullBuffer, unsigned long length, bool fromEnd);
    
    
    /**
     * Performs sorting of lines array
     */
    void (*sort)(struct SortedLinesContainer* this);
    
    
    /**
     * Allocates memory so that lines object can contain one more element
     * Uses allocIncrement
     * @param this object to be constructed
     */
    int (*allocate)(struct SortedLinesContainer* this);
    
    
    /**
     * Frees space
     * @param this object to be constructed
     */
    void (*free)(struct SortedLinesContainer* this);
} SortedLinesContainer;


/**
 * Check SortedLinesContainer compar for information
 */
int compar(const void *, const void*);


/**
 * Check SortedLinesContainer comparRev for information
 */
int comparRev(const void *, const void*);


/**
 * Check SortedLinesContainer sort for information
 */
void sortContainer(struct SortedLinesContainer* this);


/**
 * Check SortedLinesContainer allocate for information
 */
int allocateContainer(struct SortedLinesContainer* this);


/**
 * Check SortedLinesContainer construct for information
 */
int construct(struct SortedLinesContainer* this,
              char* fullBuffer, unsigned long length, bool fromEnd);


/**
 * Constructs SortedLinesContainer from file
 */
int constructFromFile(struct SortedLinesContainer* this, char* fileName, bool fromEnd);


/**
 * Sets default values for SortedLinesContainer
 */
void defaultContainer(struct SortedLinesContainer* this);


/**
 * Check SortedLinesContainer free for information
 */
void freeContainer(struct SortedLinesContainer* this);


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
 * Creates a line copy without punctuation marks.
 * @param lineIn [in] input line
 * @param lineOut [out] output line
 */
void lineWithoutPunctuation(string* lineIn, string* lineOut);


/**
 * Tests for lineWithoutPunctuation().
 * @return if valid
 */
bool tests_lineWithoutPunctuation(void);


/**
 * Checks whether string consists only of invisible characters.
 * @param line string to check
 * @return if has some visible content
 */
bool hasVisibleContent(string line);


/**
 * Tests for hasVisibleContent().
 * @return if valid
 */
bool tests_hasVisibleContent(void);


/**
 * Outputs container line-by-line to the file
 * @param this [in] container
 * @param fileName [in] name of output file
 */
unsigned long outputContainer(SortedLinesContainer* this, char* fileName);


/**
 * Tests for SortedLinesContainer
 * @return success
 */
bool testContainer(void);


/**
 * Trims unprintable symbols from both ends of string.
 * @param line string to be modified
 */
void trimUnprintable(string* line);


/**
 * Tests for trimUnprintable()
 */
bool tests_trimUnprintable(void);


/**
 * Is char is visible
 * @param c char to be checked
 */
bool isprintable(char c);


/**
 * Performs all tests and returns true if valid
 */
bool performAllTests(void);


int main(int argc, const char * argv[]) {
    bool reversed = false;
    bool runTests = false;
    char* inputFileName = calloc(10, sizeof(char));
    char* outputFileName = calloc(11, sizeof(char));
    
    strcpy(inputFileName, "input.txt");
    strcpy(outputFileName, "output.txt");
    
    for(int i = 0; i < argc; i++){
        if (strcmp("-r", argv[i]) == 0){
            reversed = true;
            continue;
        }
        
        if (strcmp("-test", argv[i]) == 0){
            runTests = true;
            continue;
        }
        
        if (strcmp("-output", argv[i]) == 0){
            i++;
            unsigned long len = strlen(argv[i]);
            char* outputFileNameNew = realloc(outputFileName, sizeof(char) * (len + 1));
            if (outputFileNameNew == NULL){
                printf("Can't realloc memory for output file name\n");
                free(outputFileName);
                return EXIT_FAILURE;
            } else {
                outputFileName = outputFileNameNew;
            }
            strcpy(outputFileName, argv[i]);
            i++;
            continue;
        }
        if (strcmp("-input", argv[i]) == 0){
            i++;
            unsigned long len = strlen(argv[i]);
            
            char* inputFileNameNew = realloc(inputFileName, sizeof(char) * (len + 1));
            if (inputFileNameNew == NULL){
                printf("Can't realloc memory for input file name\n");
                free(inputFileName);
                return EXIT_FAILURE;
            } else {
                inputFileName = inputFileNameNew;
            }
            
            strcpy(inputFileName, argv[i]);
            i++;
            continue;
        }
    }
    printf("Aleksandr Dremov\n"
           "(c) 2020 all rights reserved\n\n");
    if (runTests){
        if (performAllTests()){
            printf("All tests passed\n");
        } else {
            printf("Some tests failed!\n");
            free(inputFileName);
            free(outputFileName);
            return EXIT_FAILURE;
        }
    }
    printf("Will sort %s file and output to %s\n", inputFileName, outputFileName);
    printf("Is sort from the end: %s\n", (reversed) ? "true" : "false");
    
    SortedLinesContainer container;
    container.construct = &construct;
    
    int constructResult = constructFromFile(&container, inputFileName, reversed);
    if (constructResult == EXIT_FAILURE) {
        printf("Failed constructFromFile()\n");
        free(inputFileName);
        free(outputFileName);
        return constructResult;
    }
    
    unsigned long linesWrote = outputContainer(&container, outputFileName);
    free(inputFileName);
    free(outputFileName);
     container.free(&container);
    if (linesWrote == 0){
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}


bool performAllTests(){
    return tests_hasVisibleContent() && tests_lineWithoutPunctuation() && tests_trimUnprintable();
}


bool hasVisibleContent(string line) {
    for (unsigned long i = 0; i < line.len; i++){
        if (isprintable(line.contents[i])){
            return true;
        }
    }
    return false;
}

bool tests_hasVisibleContent(){
    string inputs[] = {
        {" ", 1, false},
        {"", 0, false},
        {"asd", 3, false},
        {"a", 1, false},
        {"\n", 1, false},
        {"\0", 1, false}
    };
    
    bool outputs[] = {
        false,
        false,
        true,
        true,
        false,
        false
    };
    
    assert(sizeof(outputs)/sizeof(bool) == sizeof(inputs)/sizeof(string));
    
    int totalNumber = sizeof(outputs)/sizeof(bool);
    bool valid = true;
    
    for (int i = 0; i<totalNumber; i++) {
        bool actualOutput = hasVisibleContent(inputs[i]);
        if (!(actualOutput == outputs[i])) {
            printf("Failed hasVisibleContent test #(%d) %d!=%d\n", i+1, actualOutput, outputs[i]);
            valid = false;
        }
    }
    
    return valid;
}


int construct(struct SortedLinesContainer* this, char* fullBuffer, unsigned long length, bool fromEnd) {
    assert(this != NULL);
    assert(fullBuffer != NULL);
    
    this->fullBuffer = calloc(length + 1, sizeof(char));
    if (!this->fullBuffer){
        printf("Can't allocate space for fullBuffer in construct()\n");
        this->free(this);
        return EXIT_FAILURE;
    }
    
    this->fromEnd = fromEnd;
    
    unsigned long curCounter = 0;
    for (unsigned long i = 0; i < length; i++){
        this->fullBuffer[i] = fullBuffer[i];
        if (curCounter == 0){
            this->allocate(this);
            string newLocated = {(this->fullBuffer + i), 0, false};
            this->lines[this->linesNumber] = newLocated;
            this->linesNumber += 1;
            curCounter++;
            continue;
        }
        if (this->fullBuffer[i] == '\n' || this->fullBuffer[i] == '\0') {
            this->fullBuffer[i] = '\0';
            (this->lines + this->linesNumber - 1)->len = curCounter;
            
            trimUnprintable((this->lines + this->linesNumber - 1));
            
            if (!hasVisibleContent(*(this->lines + this->linesNumber - 1))) {
                this->linesNumber -= 1;
            }
            curCounter = 0;
            continue;
        }
        curCounter++;
    }
    if (curCounter != 0){
        (this->lines + this->linesNumber - 1)->len = curCounter;
    }
    this->fullBuffer[length] = '\0';
    return EXIT_SUCCESS;
}


int constructFromFile(struct SortedLinesContainer* this, char* fileName, bool fromEnd){
    char * buffer = 0;
    unsigned long length = 0;
    FILE * fp = fopen (fileName, "r");
    
    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        length = (unsigned long) ftell(fp);
        fseek (fp, 0, SEEK_SET);
        buffer = calloc(length, sizeof(char));
        if (buffer)
        {
            fread (buffer, 1, length, fp);
        }else{
            printf("Can't allocate memory "
                   "for buffer in constructFromFile()\n");
            free(buffer);
            this->free(this);
            return EXIT_FAILURE;
        }
        fclose (fp);
        unsigned long lenElements = length / sizeof(char);
        defaultContainer(this);
        this->lines = calloc(lenElements + 1, sizeof(string));
        this->linesMaxNumber = lenElements + 1;
        
        int constructResult = this->construct(this, buffer, lenElements, fromEnd);
        if (constructResult == EXIT_FAILURE) {
            printf("Failed constructing object in constructFromFile()\n");
            free(buffer);
            return EXIT_FAILURE;
        }
        this->sort(this);
        free(buffer);
    } else {
        printf("Can't open %s for read\n", fileName);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


bool testContainer(){
    return true;
}


void freeContainer(struct SortedLinesContainer* this){
    for (unsigned long i = 0; i < this->linesNumber; i++){
        if (this->lines[i].allocated)
            free(this->lines[i].contents);
    }
    if (this->lines)
        free(this->lines);
    
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


int allocateContainer(struct SortedLinesContainer* this) {
    assert(this != NULL);
    
    if (!this->lines || this->linesMaxNumber == 0){
        this->lines = calloc(this->allocIncrement, sizeof(string));
        
        if (this->lines == NULL){
            printf("Failed allocating memory in allocateContainer()\n");
            this->free(this);
            return EXIT_FAILURE;
        }
        this->linesMaxNumber = this->allocIncrement;
    } else {
        if (this->linesNumber + 1 >= this->linesMaxNumber){
            this->linesMaxNumber += this->allocIncrement;
            string* newAllocation = realloc(this->lines, this->linesMaxNumber * sizeof(string));
            if (newAllocation == NULL){
                printf("Failed reallocating memory in allocateContainer()\n");
                this->free(this);
                return EXIT_FAILURE;
            } else {
                this->lines = newAllocation;
            }
            
        }
    }
    return EXIT_SUCCESS;
}


int compar(const void* line1, const void* line2){
    assert(line1 != NULL);
    assert(line2 != NULL);
    assert(line1 != line2);
    
    string* line1String = (struct string *) line1;
    string* line2String = (struct string *) line2;
    
    string noPunct1, noPunct2;
    
    lineWithoutPunctuation(line1String, &noPunct1);
    lineWithoutPunctuation(line2String, &noPunct2);
    
    int compareResult = strcmp(noPunct1.contents, noPunct2.contents);

    if (noPunct1.allocated)
        free(noPunct1.contents);
    if (noPunct2.allocated)
        free(noPunct2.contents);
        
    return compareResult;
}


int comparRev(const void* line1, const void* line2){
    assert(line1 != NULL);
    assert(line2 != NULL);
    assert(line1 != line2);
    
    string* line1String = (struct string *) line1;
    string* line2String = (struct string *) line2;
    
    string noPunct1 = *line1String;
    string noPunct2 = *line2String;
    
    lineWithoutPunctuation(line1String, &noPunct1);
    lineWithoutPunctuation(line2String, &noPunct2);
    
    char* tmpLine1 = calloc(noPunct1.len + 1, sizeof(char));
    char* tmpLine2 = calloc(noPunct2.len + 1, sizeof(char));
    
    if (tmpLine1 == NULL || tmpLine2 == NULL){
        printf("Failed allocating memory in comparRev()\n");
        return EXIT_FAILURE;
    }
    
    strcpy(tmpLine1, noPunct1.contents);
    strcpy(tmpLine2, noPunct2.contents);
    reverse(tmpLine1, noPunct1.len);
    reverse(tmpLine2, noPunct2.len);
    
    
    int compareResult = strcmp(tmpLine1, tmpLine2);
    
    free(tmpLine1);
    free(tmpLine2);
    
    if (noPunct1.allocated)
        free(noPunct1.contents);
    if (noPunct2.allocated)
        free(noPunct2.contents);
    
    return compareResult;
}


void reverse(char* line, unsigned long len) {
    assert(line != NULL);
    
    if (len <= 1)
        return;
    
    char* begin = line;
    char* end = line + len - 1;
    char c = 0;
    for (unsigned long i = 0; i < len / 2; i++){
        c = *end;
        *end = *begin;
        *begin = c;
        
        begin++;
        end--;
    }
}


void defaultContainer(struct SortedLinesContainer* this) {
    this->allocIncrement = 1024;
    this->linesNumber = 0;
    this->fromEnd = false;
    this->linesMaxNumber = 0;
    this->allocate = &allocateContainer;
    this->compar = &compar;
    this->comparRev = &comparRev;
    this->sort = &sortContainer;
    this->free = &freeContainer;
}


void lineWithoutPunctuation(string* lineIn, string* lineOut) {
    assert(lineIn != NULL);
    assert(lineOut != NULL);
    assert(lineOut != lineIn);
    
    lineOut->contents = calloc(lineIn->len + 1, sizeof(char));
    lineOut->allocated = true;
    
    char* outPos = lineOut->contents;
    for (unsigned long i = 0; i < lineIn->len; i++){
        if (ispunct(lineIn->contents[i]) || lineIn->contents[i] == '\n')
            continue;
        *(outPos++) = lineIn->contents[i];
    }
    
    lineOut->len = (unsigned long)(outPos - lineOut->contents);
}


bool tests_lineWithoutPunctuation(){
    string inputs[] = {
        {" aasd,", 6, false},
        {"", 0, false},
        {"asd  ", 5, false},
        {"-,a:,", 5, false}
    };
    
    string outputs[] = {
        {" aasd", 5, false},
        {"", 0, false},
        {"asd  ", 5, false},
        {"a", 1, false},
    };
    
    assert(sizeof(outputs)/sizeof(string) == sizeof(inputs)/sizeof(string));
    
    int totalNumber = sizeof(outputs)/sizeof(string);
    bool valid = true;
    
    for (int i = 0; i < totalNumber; i++) {
        string out = {"", 0, false};
        lineWithoutPunctuation(&inputs[i], &out);
        if (!(strcmp(out.contents, outputs[i].contents) == 0 && out.len == outputs[i].len)) {
            printf("Failed lineWithoutPunctuation test #(%d) %s!=%s\n", i+1, out.contents, outputs[i].contents);
            valid = false;
        }
        free(out.contents);
    }
    
    return valid;
}


void trimUnprintable(string* line) {
    char* oldStart = line->contents;
    char* pos = oldStart;
    unsigned long i = 0;
    
    for (; i < line->len; i++){
        if (!(isprintable(*pos))){
            pos++;
        } else {
            break;
        }
    }
    
    line->contents = pos;
    line->len = line->len - i;
    
    for (i = line->len; i > 0; i--){
        if (isprintable(line->contents[i])){
            line->contents[i+1] = '\0';
            line->len = i+1;
            break;
        }
    }
    if (i == 0){
        line->len = 1;
        if (line->contents[0] == '\0')
            line->len = 0;
        if (line->len >= 1) // not counting \0
            line->contents[1] = '\0';
    }
}


bool tests_trimUnprintable(){
    string inputs[] = {
        {" \n\raasd ", 8, false},
        {"\n  345", 6, false},
        {"asd  ", 5, false},
        {"a", 1, false},
        {"", 0, false},
        {"    ", 4, false},
        {"a    ", 5, false}
    };
    
    string outputs[] = {
        {"aasd", 4, false},
        {"345", 3, false},
        {"asd", 3, false},
        {"a", 1, false},
        {"", 0, false},
        {"", 0, false},
        {"a", 1, false},
    };
    
    assert(sizeof(outputs)/sizeof(string) == sizeof(inputs)/sizeof(string));
    
    int totalNumber = sizeof(outputs)/sizeof(string);
    bool valid = true;
    
    string out;
    for (int i = 0; i < totalNumber; i++) {
        out = inputs[i];
        out.contents = calloc(out.len + 1, sizeof(char));
        char *initalLocated = out.contents;
        
        strcpy(out.contents, inputs[i].contents);
        trimUnprintable(&out);
        if (!(strcmp(out.contents, outputs[i].contents) == 0 && out.len == outputs[i].len)) {
            printf("Failed trimUnprintable test #(%d) %s!=%s or %lu!=%lu\n", i+1, out.contents, outputs[i].contents, out.len, outputs[i].len);
            valid = false;
        }
        
        free(initalLocated);
    }
    
    return valid;
}



bool isprintable(char c) {
    return !(c == '\n' || c == '\t' || c == ' ' || c == '\r' || c == '\0');
}


unsigned long outputContainer(SortedLinesContainer* this, char* fileName) {
    assert(this != NULL);
    
    FILE* fp = fopen(fileName, "w");
    
    if (fp == NULL){
        printf("Could not open %s file\n", fileName);
        return 0;
    }
    
    for(unsigned long i = 0; i < this->linesNumber; i++){
        fputs((this->lines + i)->contents, fp);
        fputs("\n", fp);
    }
    
    fclose(fp);
    return this->linesNumber;
}
