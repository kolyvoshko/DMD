#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include "parsefile.h"
#include "iniparser.h"

#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wunused-function"

/**
 * This enum stores the status for each parsed line (internal use only).
 */
typedef enum _line_status_ {
    LINE_UNPROCESSED,
    LINE_ERROR,
    LINE_EMPTY,
    LINE_COMMENT,
    LINE_SECTION,
    LINE_VALUE
} line_status ;

struct _KeyVal{
    char*   section;
    char*   key;
    double  val;
};

static char**   readFile        (char* filename);
static char**   readLines       (FILE* txt, int* count);
static _KeyVal* readToKeyList   (FILE* txt, int* count);
static double   getKeyDouble    (char* key, double def_val, char** massKey);
static int      getKeyInt       (char* key, int def_val, char** massKey);
static char*    getKeyChar      (char* key, char* def_val, char** massKey);

void printFile(){
}

void parseFile(char* filename){
    char**      array    = NULL;    /* Array of read lines */
    FILE*       file     = NULL;    /* File to read lines from */
    int         i;                  /* Loop index */
    int         line_count;         /* Total number of read lines */

    file = fopen(filename, "rt");
    if (file == NULL) {
        printf("Can't open file %s.\n", filename);
    }

    /* Read lines from file. */
    array = readLines(file, &line_count);

    double test;
    test = getKeyDouble   ("Run", 50, array);
    printf("%lf", test);

    /* Just for test, print out the read lines. */
    for (i = 0; i < line_count; i++) {
        printf("[%d]:\t %s\n", (i+1), array[i]);
    }

    /* Cleanup. */
    fclose(file);
    for (i = 0; i < line_count; i++) {
        free(array[i]);
    }
    free(array);
    array = NULL;
}

static char** readLines(FILE* txt, int* count) {
    char** array = NULL;        /* Array of lines */
    int    i;                   /* Loop counter */
    char   line[100];           /* Buffer to read each line */
    int    line_count;          /* Total number of lines */
    int    line_length;         /* Length of a single line */

    /* Clear output parameter. */
    *count = 0;

    /* Get the count of lines in the file */
    line_count = 0;
    while (fgets(line, sizeof(line), txt) != NULL) {
       line_count++;
    }

    /* Move to the beginning of file. */
    rewind(txt);

    /* Allocate an array of pointers to strings
     * (one item per line). */
    array = malloc(line_count * sizeof(char *));
    if (array == NULL) {
        return NULL;
    }

    /* Read each line from file and deep-copy in the array. */
    for (i = 0; i < line_count; i++) {
        fgets(line, sizeof(line), txt);

        /* Remove the ending '\n' from the read line. */
        line_length = strlen(line);
        line[line_length - 1] = '\0';
        line_length--; /* update line length */

        /* Allocate space to store a copy of the line. +1 for NUL terminator */
        array[i] = malloc(line_length + 1);

        /* Copy the line into the newly allocated space. */
        strcpy(array[i], line);
    }

    /* Write output param */
    *count = line_count;

    return array;
}

static _KeyVal* readToKeyList (FILE* txt, int* count); {
    char** array = NULL;        /* Array of lines */
    _KeyVal* arrKeyVal;
    int    i;                   /* Loop counter */
    char   line[100];           /* Buffer to read each line */
    int    line_count;          /* Total number of lines */
    int    line_length;         /* Length of a single line */

    /* Clear output parameter. */
    *count = 0;

    /* Get the count of lines in the file */
    line_count = 0;
    while (fgets(line, sizeof(line), txt) != NULL) {
       line_count++;
    }

    /* Move to the beginning of file. */
    rewind(txt);

    /* Allocate an array of pointers to strings
     * (one item per line). */
    array = malloc(line_count * sizeof(char *));
    if (array == NULL) {
        return NULL;
    }

    /* Read each line from file and deep-copy in the array. */
    for (i = 0; i < line_count; i++) {
        fgets(line, sizeof(line), txt);

        /* Remove the ending '\n' from the read line. */
        line_length = strlen(line);
        line[line_length - 1] = '\0';
        line_length--; /* update line length */

        /* Allocate space to store a copy of the line. +1 for NUL terminator */
        array[i] = malloc(line_length + 1);

        /* Copy the line into the newly allocated space. */
        strcpy(array[i], line);
    }

    /* Write output param */
    *count = line_count;

    return array;
}

static double getKeyDouble(char* key, double def_val, char** array){
    int i;
    double val = 0;
    char* tmp_key = NULL;
    for (i=0; array[i] != '\0'; i++){
        sscanf("")
        if (strcmp(array[i], key) == 0 ){
            return val;
        }
        else{
            return def_val;
        }
    }
}

static int getKeyInt(char* key, int def_val, char** massKey){
    int val;
    return val;
}

static char* getKeyChar(char* key, char* def_val, char** massKey){
    char* val;
    return val;
}





