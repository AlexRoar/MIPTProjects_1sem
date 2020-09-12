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
    char *contents;

    unsigned long len;

    /**
     * Whether contents were alocated by malloc (can be freed) or just a reference
     */
    bool allocated;
} string;


typedef struct SortedLinesContainer {
    /**
     * Pointer to the start of the string containing substrings separated by \0
     * (\n replaced by \0)
     */
    char *fullBuffer;

    /**
     * Strings containing starts of substrings in fullBuffer
     */
    string *lines;

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
    int (*compar)(const void *line1, const void *line2);

    /**
     * Function used in qsort for comparison from the end of the string
     * @param line1 first  struct string object
     * @param line2 second struct string object
     */
    int (*comparRev)(const void *line1, const void *line2);

    /**
     * Constructs default object
     * Invisible charecters are trimmed and blank lines are removed
     * @param this object to be constructed
     * @param fullBuffer pointer to the first char of readed file
     * @param length length of the buffer
     * @param fromEnd whether to sort from the end
     */
    int (*construct)(struct SortedLinesContainer *this, const char *fullBuffer, const unsigned long length,
                     const bool fromEnd);

    /**
     * Performs sorting of lines array
     */
    void (*sort)(struct SortedLinesContainer *this);

    /**
     * Allocates memory so that lines object can contain one more element
     * Uses allocIncrement
     * @param this object to be constructed
     */
    int (*allocate)(struct SortedLinesContainer *this);

    /**
     * Frees space
     * @param this object to be constructed
     */
    void (*free)(struct SortedLinesContainer *this);
} SortedLinesContainer;


/**
 * Check SortedLinesContainer compar for information
 */
int compar(const void *, const void *);


/**
 * Check SortedLinesContainer comparRev for information
 */
int comparRev(const void *, const void *);


/**
 * Compare strings lexicographically, removing punctuation, trimming unprintable, and removing double whitespaces.
 * @param line1 first line
 * @param line2 second line
 * @param fromEnd Whether to compare from the end
 */
int multiCompare(const void *line1, const void *line2, const int fromEnd);


/**
 * Set lineSleep to 1 if current position is somewhere between reacurring whitespaces
 * @param line line to be checked
 * @param lineCurrent current position
 * @param modifier 1 or -1 - direction of scaning (forwards or backwards)
 */
void doubleWhitespacesSkip(const string *line, const char *lineCurrent, short *lineSleep, const int modifier);


/**
 * Tests for doubleWhitespacesSkip()
 */
int tests_multiCompare(void);


/**
 * Check SortedLinesContainer sort for information
 */
void sortContainer(struct SortedLinesContainer *this);


/**
 * Check SortedLinesContainer allocate for information
 */
int allocateContainer(struct SortedLinesContainer *this);


/**
 * Check SortedLinesContainer construct for information
 */
int construct(struct SortedLinesContainer *this,
              const char *fullBuffer, const unsigned long length, const bool fromEnd);


/**
 * Constructs SortedLinesContainer from file
 */
int constructFromFile(struct SortedLinesContainer *this, const char *fileName, const bool fromEnd);


/**
 * Sets default values for SortedLinesContainer
 */
void defaultContainer(struct SortedLinesContainer *this);


/**
 * Check SortedLinesContainer free for information
 */
void freeContainer(struct SortedLinesContainer *this);


/**
 * Outputs container line-by-line to the file
 * @param this [in] container
 * @param fileName [in] name of output file
 */
unsigned long outputContainer(SortedLinesContainer *this, char *fileName);


/**
 * Tests for SortedLinesContainer
 * @return success
 */
bool testContainer(void);


/**
 * Is char is visible
 * @param c char to be checked
 */
bool isprintable(char c);


/**
 * Is char is punctuation
 * @param c char to be checked
 */
bool ispunctuation(char c);


/**
 * Performs all tests and returns true if valid
 */
bool performAllTests(void);


/**
 * Line analyzer. Fires sleep if current symbol is not ready to be compared with something (skip this symbol)
 * @param line full analyzed string
 * @param lineCurrent line current position
 * @param insideLine whether inside line (leading whitespaces skipped)
 * @param lineSleep whether to skip current symbol
 */
void lineAnalyze(const string *line, char *lineCurrent, short *insideLine, short *lineSleep);


/**
 * Decreases line length so that to ignore trailing whitespaces
 * @param line full analyzed string
 */
void adjustLenTrimmingWhitespaces(string *line);


int main(int argc, const char *argv[]) {
    bool reversed = false;
    bool runTests = false;
    char *inputFileName = calloc(10, sizeof(char));
    char *outputFileName = calloc(11, sizeof(char));

    strcpy(inputFileName, "input.txt");
    strcpy(outputFileName, "output.txt");

    for (int i = 0; i < argc; i++) {
        if (strcmp("-r", argv[i]) == 0) {
            reversed = true;
            continue;
        }

        if (strcmp("-test", argv[i]) == 0) {
            runTests = true;
            continue;
        }

        if (strcmp("-output", argv[i]) == 0) {
            i++;
            unsigned long len = strlen(argv[i]);
            char *outputFileNameNew = realloc(outputFileName, sizeof(char) * (len + 1));
            if (outputFileNameNew == NULL) {
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
        if (strcmp("-input", argv[i]) == 0) {
            i++;
            unsigned long len = strlen(argv[i]);

            char *inputFileNameNew = realloc(inputFileName, sizeof(char) * (len + 1));
            if (inputFileNameNew == NULL) {
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
    if (runTests) {
        if (performAllTests()) {
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
    if (linesWrote == 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


bool performAllTests() {
    return tests_multiCompare();
}


int construct(struct SortedLinesContainer *this, const char *fullBuffer,
              const unsigned long length, const bool fromEnd) {
    assert(this != NULL);
    assert(fullBuffer != NULL);

    this->fullBuffer = calloc(length + 1, sizeof(char));
    if (!this->fullBuffer) {
        printf("Can't allocate space for fullBuffer in construct()\n");
        this->free(this);
        return EXIT_FAILURE;
    }

    this->fromEnd = fromEnd;

    unsigned long curCounter = 0;
    for (unsigned long i = 0; i < length; i++) {
        this->fullBuffer[i] = fullBuffer[i];
        if (curCounter == 0) {
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
            curCounter = 0;
            continue;
        }
        curCounter++;
    }
    if (curCounter != 0) {
        (this->lines + this->linesNumber - 1)->len = curCounter;
    }
    this->fullBuffer[length] = '\0';
    return EXIT_SUCCESS;
}


int constructFromFile(struct SortedLinesContainer *this, const char *fileName, const bool fromEnd) {
    char *buffer = 0;
    unsigned long length = 0;
    FILE *fp = fopen(fileName, "r");

    if (fp) {
        fseek(fp, 0, SEEK_END);
        length = (unsigned long) ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = calloc(length, sizeof(char));
        if (buffer) {
            fread(buffer, 1, length, fp);
        } else {
            printf("Can't allocate memory "
                   "for buffer in constructFromFile()\n");
            free(buffer);
            this->free(this);
            return EXIT_FAILURE;
        }
        fclose(fp);
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


bool testContainer() {
    return true;
}


void freeContainer(struct SortedLinesContainer *this) {
    for (unsigned long i = 0; i < this->linesNumber; i++) {
        if (this->lines[i].allocated)
            free(this->lines[i].contents);
    }
    if (this->lines)
        free(this->lines);

    if (this->fullBuffer)
        free(this->fullBuffer);
}


void sortContainer(struct SortedLinesContainer *this) {
    if (!this->fromEnd) {
        qsort(this->lines, this->linesNumber, sizeof(string), this->compar);
    } else {
        qsort(this->lines, this->linesNumber, sizeof(string), this->comparRev);
    }
}


int allocateContainer(struct SortedLinesContainer *this) {
    assert(this != NULL);

    if (!this->lines || this->linesMaxNumber == 0) {
        this->lines = calloc(this->allocIncrement, sizeof(string));

        if (this->lines == NULL) {
            printf("Failed allocating memory in allocateContainer()\n");
            this->free(this);
            return EXIT_FAILURE;
        }
        this->linesMaxNumber = this->allocIncrement;
    } else {
        if (this->linesNumber + 1 >= this->linesMaxNumber) {
            this->linesMaxNumber += this->allocIncrement;
            string *newAllocation = realloc(this->lines, this->linesMaxNumber * sizeof(string));
            if (newAllocation == NULL) {
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


int compar(const void *line1, const void *line2) {
    assert(line1 != NULL);
    assert(line2 != NULL);
    assert(line1 != line2);

    return multiCompare(line1, line2, 0);
}


int comparRev(const void *line1, const void *line2) {
    assert(line1 != NULL);
    assert(line2 != NULL);
    assert(line1 != line2);

    return multiCompare(line1, line2, 1);
}

int multiCompare(const void *line1, const void *line2, const int fromEnd) {
    string *line1String = (struct string *) line1;
    string *line2String = (struct string *) line2;

    // Strings are not copied. Just a reference with different length
    string tmpString1 = {line1String->contents, line1String->len, line1String->allocated};
    string tmpString2 = {line2String->contents, line2String->len, line2String->allocated};

    adjustLenTrimmingWhitespaces(&tmpString1);
    adjustLenTrimmingWhitespaces(&tmpString2);


    char *line1Current = line1String->contents;
    char *line2Current = line2String->contents;

    short insideLine1 = 0, insideLine2 = 0;
    short line1Ended = 0, line2Ended = 0;
    short line1Sleep = 1, line2Sleep = 1;

    short int modifier = 1;
    if (fromEnd) {
        modifier = -1;
        line1Current += tmpString1.len;
        if (tmpString1.len != 0)
            line1Current -= 1;

        line2Current += tmpString2.len;
        if (tmpString2.len != 0)
            line2Current -= 1;
    }

    while (1) {
        if (*line1Current == '\0' || line1Ended || line1Current >= tmpString1.contents + tmpString1.len) {
            line1Ended = 1;
            line1Sleep = 1;
        } else {
            lineAnalyze(&tmpString1, line1Current, &insideLine1, &line1Sleep);
            doubleWhitespacesSkip(&tmpString1, line1Current, &line1Sleep, modifier);
        }

        if (*line2Current == '\0' || line2Ended || line2Current >= tmpString2.contents + tmpString2.len) {
            line2Ended = 1;
            line2Sleep = 1;
        } else {
            lineAnalyze(&tmpString2, line2Current, &insideLine2, &line2Sleep);
            doubleWhitespacesSkip(&tmpString2, line2Current, &line2Sleep, modifier);
        }

        if (line1Ended == 1 || line2Ended == 1)
            break;

        if (line1Sleep == 1 && line2Sleep == 0) {
            line1Current += modifier;
        } else if (line1Sleep == 0 && line2Sleep == 1) {
            line2Current += modifier;
        } else if (line1Sleep == 1 && line2Sleep == 1) {
            line1Current += modifier;
            line2Current += modifier;
        } else {
            if (*line1Current == *line2Current) {
                line1Current += modifier;
                line2Current += modifier;
                continue;
            }

            return *line1Current - *line2Current;
        }
        if (modifier == -1 && line1Current == line1String->contents) {
            line1Ended = 1;
            line1Sleep = 1;
        }
        if (modifier == -1 && line2Current == line2String->contents) {
            line1Ended = 1;
            line1Sleep = 1;
        }
    }
    if (line1Ended == line2Ended)
        return 0;
    else {
        return line1Ended - line2Ended;
    }
}


int tests_multiCompare(void) {
    string inputs1[] = {
            {" ",      0, false},
            {"   a",   0, false},
            {"   a  ", 0, false},
            {"b",      0, false},
            {"\"b",    0, false},
    };
    string inputs2[] = {
            {" ", 0, false},
            {"a", 0, false},
            {"a", 0, false},
            {"a", 0, false},
            {"b", 0, false}
    };

    int outputs[] = {
            0,
            0,
            0,
            1,
            0
    };

    assert(sizeof(outputs) / sizeof(int) == sizeof(inputs1) / sizeof(string));

    int totalNumber = sizeof(outputs) / sizeof(int);
    bool valid = true;

    for (int i = 0; i < totalNumber; i++) {
        inputs1[i].len = strlen(inputs1[i].contents);
        inputs2[i].len = strlen(inputs2[i].contents);
        int actualOutput = multiCompare(&inputs1[i], &inputs2[i], 0);
        if (!((actualOutput == outputs[i] && outputs[i] == 0) || (outputs[i] > 0 && actualOutput > 0) ||
              (outputs[i] < 0 && actualOutput < 0))) {
            printf("Failed tests_multiCompare test #(%d) %d !~ %d\n", i + 1, actualOutput, outputs[i]);
            valid = false;
        }
    }

    return valid;
}


void doubleWhitespacesSkip(const string *line, const char *lineCurrent, short *lineSleep, const int modifier) {
    if (modifier > 0) {
        if (lineCurrent >= (line->contents + 1)) {
            if (*lineCurrent == ' ' && *(lineCurrent - 1) == ' ')
                *lineSleep = 1;
        }
    } else {
        if (lineCurrent <= (line->contents + line->len - 1)) {
            if (*lineCurrent == ' ' && *(lineCurrent + 1) == ' ')
                *lineSleep = 1;
        }
    }
}


void adjustLenTrimmingWhitespaces(string *line) {
    char *pos = line->contents + line->len;
    if (line->len == 0)
        return;
    pos--;
    while (pos != line->contents) {
        if (isprintable(*pos))
            break;
        pos--;
        line->len--;
    }
}


void lineAnalyze(const string *line, char *lineCurrent, short *insideLine, short *lineSleep) {
    if (!isprintable(*lineCurrent) && !*insideLine) {
        *lineSleep = 1;
        return;
    }
    if (isprintable(*lineCurrent) && !*insideLine) {
        *insideLine = 1;
        *lineSleep = 0;
    }
    if (ispunctuation(*lineCurrent)) {
        *lineSleep = 1;
    } else {
        *lineSleep = 0;
    }
}


void defaultContainer(struct SortedLinesContainer *this) {
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


bool isprintable(char c) {
    return !(c == '\n' || c == '\t' || c == ' ' || c == '\r' || c == '\0');
}


bool ispunctuation(char c) {
    return ((c >= '!' && c < '0') || c == '\'' || c == '"' || c == '`');
}


unsigned long outputContainer(SortedLinesContainer *this, char *fileName) {
    assert(this != NULL);

    FILE *fp = fopen(fileName, "w");

    if (fp == NULL) {
        printf("Could not open %s file\n", fileName);
        return 0;
    }

    for (unsigned long i = 0; i < this->linesNumber; i++) {
        fputs((this->lines + i)->contents, fp);
        fputs("\n", fp);
    }

    fclose(fp);
    return this->linesNumber;
}
