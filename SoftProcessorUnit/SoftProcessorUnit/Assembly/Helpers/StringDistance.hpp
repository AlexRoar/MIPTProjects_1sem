//
//  StringDistance.hpp
//  SPUAsm
//
//  Created by Александр Дремов on 15.10.2020.
//

#ifndef StringDistance_hpp
#define StringDistance_hpp

#include <stdio.h>

typedef enum {
    INSERTION,
    DELETION,
    SUBSTITUTION,
    NONE
} edit_type;
 
struct edit {
    unsigned int score;
    edit_type type;
    char arg1;
    char arg2;
    unsigned int pos;
    struct edit *prev;
};
typedef struct edit edit;


static int min3(int a, int b, int c);

static unsigned int levenshtein_matrix_calculate(edit **mat, const char *str1, size_t len1,
                                                 const char *str2, size_t len2);

static edit **levenshtein_matrix_create(const char *str1, size_t len1, const char *str2,
                                        size_t len2);

unsigned int levenshtein_distance(const char *str1, const char *str2, edit **script);
#endif /* StringDistance_hpp */

