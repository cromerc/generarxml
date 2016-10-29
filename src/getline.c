#ifdef __MINGW32__
#include <stdio.h>
#include <stdlib.h>
#include "getline.h"

size_t getline(char **lineptr, size_t *n, FILE *stream) {
    int ch;
    size_t chars = 0;
    char *line = *lineptr;

    /* why even call this if you don't have a stream to read? */
    if (stream == NULL) {
    	return -1;
    }

    ch = fgetc(stream);
    if (ch == EOF) {
    	return -1;
    }
    
    if (line == NULL) {
        if (*n == 0) {
            line = (char *) calloc(GET_LINE_MAX_MEM, sizeof(char));
            *n = GET_LINE_MAX_MEM;
        }
        else {
            line = (char *) calloc(*n, sizeof(char));
        }

        /* out of memory */
        if (line == NULL) {
            return -1;
        }
    }
    else {
        free(line);
        *n = GET_LINE_MAX_MEM;
        line = (char *) calloc(GET_LINE_MAX_MEM, sizeof(char));
    }
    
    while(ch != EOF) {
        if (chars == *n) {
            /* not enough memory for more chars, add more */
            *n = *n + GET_LINE_MAX_MEM;
            line = (char *) realloc(line, *n * sizeof(char));
        }
    	if (ch == '\n') {
            if (chars == *n - 1) {
                /* add space for the null terminator */
                *n = *n + 1;
                line = (char *) realloc(line, *n * sizeof(char));
            }
            line[chars] = '\n';
            line[chars + 1] = '\0';
    		break;
    	}
        line[chars] = ch;
        ch = fgetc(stream);
        chars++;
    }
    if (ch == EOF) {
        if (chars == *n) {
            /* add space for the null terminator */
            *n = *n + 1;
            line = (char *) realloc(line, *n * sizeof(char));
        }
        line[chars] = '\0';
    }

    *lineptr = line;

    return chars + 1;
}

size_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream) {
    int ch;
    size_t chars = 0;
    char *line = *lineptr;

    /* why even call this if you don't have a stream to read? */
    if (stream == NULL) {
    	return -1;
    }

    ch = fgetc(stream);
    if (ch == EOF) {
    	return -1;
    }
    
    if (line == NULL) {
        if (*n == 0) {
            line = (char *) calloc(GET_LINE_MAX_MEM, sizeof(char));
            *n = GET_LINE_MAX_MEM;
        }
        else {
            line = (char *) calloc(*n, sizeof(char));
        }

        /* out of memory */
        if (line == NULL) {
            return -1;
        }
    }
    else {
        free(line);
        *n = GET_LINE_MAX_MEM;
        line = (char *) calloc(GET_LINE_MAX_MEM, sizeof(char));
    }

    while(ch != EOF) {
        if (chars == *n) {
            /* not enough memory for more chars, add more */
            *n = *n + GET_LINE_MAX_MEM;
            line = (char *) realloc(line, *n * sizeof(char));
        }
    	if (ch == '\n') {
            if (chars == *n - 1) {
                /* add space for the null terminator */
                *n = *n + 1;
                line = (char *) realloc(line, *n * sizeof(char));
            }
            line[chars] = delim;
            line[chars + 1] = '\0';
    		break;
    	}
        line[chars] = ch;
        ch = fgetc(stream);
        chars++;
    }
    if (ch == EOF) {
        if (chars == *n) {
            /* add space for the null terminator */
            *n = *n + 1;
            line = (char *) realloc(line, *n * sizeof(char));
        }
        line[chars] = '\0';
    }

    if (chars < *n) {
        line = (char *) realloc(line, chars + 2 * sizeof(char));
    }

    *lineptr = line;

    return chars + 1;
}
#endif
