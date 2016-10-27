#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include "main.h"
#include "readfile.h"
#include "encoding.h"

#ifdef __MINGW32__
#define GET_LINE_MAX_MEM 128
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
#endif

/*
 * This function reads Biblia.txt, searches for the information in config,
 * and stores it into an array of strings to be used later to create the xml.
 */
int read_file() {
    FILE *file = NULL;
    CHAPTER *chapter = NULL;
    int start = 0;
    int end = 0;
    int chapters = 1;
    int verse = 0;
    int length;
    int i = 0;
    int k = 0;
    int l = 0;
    char *line = NULL;
    char *temp = NULL;
    char **array = NULL;
    ssize_t chars = 0;
    size_t lines = 0;
    size_t j = 0;
    size_t new_max = MAX_LINES;
    bool matches[3];
    matches[0] = false;
    matches[1] = false;
    matches[2] = false;

    char *ch = strtok(config->chapter_numbers, "-");
    while (ch != NULL) {
        if (i == 0) {
            start = atoi(ch);
            i++;
        }
        else {
            end = atoi(ch);
        }
        ch = strtok(NULL, "-");
    }

    #ifdef DEBUG
        printf("Start chapter: %d\nEnd chapter: %d\n", start, end);
    #endif

    if (end != 0) {
        if (start > end) {
            printf("Archivo de configuración invalido!");
            return 1;
        }
        else {
            chapters = (end - start) + 1;
        }
    }

    #ifdef DEBUG
        printf("Chapters: %d\n", chapters);
    #endif

    book->chapters = chapters;
    book->current = -1;
    book->chapter = (CHAPTER **) malloc((chapters + 1) * sizeof(CHAPTER *));

    i = 0;

    file = fopen("Biblia.txt", "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    if (!(array = calloc(new_max, sizeof(*array)))) {
        fprintf(stderr, "Allocación de memoria falló.");
        return 1;
    }

    while ((chars = getline(&line, &j, file)) != -1) {
        while (chars > 0 && (line[chars - 1] == '\n' || line[chars - 1] == '\r' || line[chars - 1] == ' ' || line[chars - 1] == '\t')) {
            /* remove whitespace from end of the line */
            chars--;
            line[chars] = '\0';
        }
        line[chars] = '\0';
        array[lines] = strdup(line);
        lines++;

        /* not enough memory for more lines, time to allocate more memory */
        if (lines == new_max) {
            /* faster, but uses more ram while running */
            /*new_max = new_max * 2;*/
            /* slower, but uses less ram */
            new_max = new_max + 100;
            char **tmp = realloc(array, new_max * sizeof(*array));
            if (!tmp) {
                fprintf(stderr, "Reallocación de memoria falló.");
                return 1;
            }
            array = tmp;
        }
    }

    /* free the extra unused memory */
    if (new_max > lines) {
        char **tmp = realloc(array, lines * sizeof(*array));
        if (!tmp) {
            fprintf(stderr, "Reallocación de memoria falló.");
            return 1;
        }
        array = tmp;
    }

    if (file) {
        fclose(file);
    }
    if (line) {
        free(line);
    }

    for (j = 0; j < lines; j++) {
        if (array[j] != NULL) {
            line = latin_to_utf8(array[j]);
        }
        if (line != NULL) {
            if (strcmp(line, config->bible) == 0) {
                /* found the bible */
                matches[0] = true;
                matches[1] = false;
                matches[2] = false;
                #ifdef DEBUG
                    printf("Bible match: %lu -> %s\n", (long) j + 1, line);
                #endif
            }
            if (strcmp(line, config->book) == 0) {
                /* found the book */
                matches[1] = true;
                matches[2] = false;
                #ifdef DEBUG
                    printf("Book match: %lu -> %s\n", (long) j + 1, line);
                #endif
            }
            for (i = start; i <=end; i++) {
                length = snprintf(NULL, 0, "%d", i) + strlen(config->chapter);
                temp = (char *) malloc((length + 2) * sizeof(char));
                snprintf(temp, length + 2, "%s %d", config->chapter, i);
                if (strcmp(line, temp) == 0) {
                    /* found a chapter */
                    matches[2] = true;
                    book->current++;
                    book->chapter[book->current] = (CHAPTER *) malloc(sizeof(CHAPTER));
                    chapter = book->chapter[book->current];
                    chapter->chapter = i;
                    chapter->current = -1;
                    chapter->verses = 0;
                    chapter->verse = (char **) malloc(sizeof(char *));
                    #ifdef DEBUG
                        printf("Chapter match: %lu -> %s\n", (long) j + 1, line);
                    #endif
                }
                free(temp);
            }
            if (matches[0] == true && matches[1] == true && matches[2] == true) {
                /* 3 matches, start getting the verses */
                length = snprintf(NULL, 0, "%d", end + 1) + strlen(config->chapter);
                temp = (char *) malloc((length + 2) * sizeof(char));
                snprintf(temp, length + 2, "%s %d", config->chapter, end + 1);
                if (strcmp(line, temp) == 0) {
                    free(line);
                    free(temp);
                    line = NULL;
                    break;
                }
                if (temp) {
                    free(temp);
                }

                temp = (char *) malloc((strlen(line) + 1) * sizeof(char));
                /* If it's a verse, match */
                for (i = 0; i <= 2; i++) {
                    if (line[i] == ' ') {
                        temp[i] = '\0';

                        verse = atoi(temp);
                        if (temp) {
                            free(temp);
                        }

                        l = 0;
                        temp = (char *) malloc((strlen(line) + 1) * sizeof(char));
                        for (k = i + 1; k < strlen(line) - 1; (k++)) {
                            temp[l] = line[k];
                            l++;
                        }
                        temp[l] = '\0';

                        chapter->current++;
                        chapter->verses++;
                        chapter->verse = (char **) realloc(chapter->verse, chapter->verses * sizeof(char *));
                        chapter->verse[chapter->current] = (char *) malloc((strlen(temp) + 1) * sizeof(char));
                        memcpy(chapter->verse[chapter->current], temp, strlen(temp) + 1);

                        if (temp) {
                            free(temp);
                        }
                        break;
                    }
                    if (!isdigit(line[i])) {
                        if (temp) {
                            free(temp);
                        }
                        break;
                    }
                    else {
                        temp[i] = line[i];
                    }
                }
            }
            if (matches[0] == true && matches[1] == true && matches[2] == true && strcmp(line, "------------------------------------------------------------------------") == 0) {
                /* end of a section and we already have 3 matches, no reason to continue */
                #ifdef DEBUG
                    printf("Bible end match: %lu -> %s\n", (long) j + 1, line);
                #endif
                free(line);
                line = NULL;
                break;
            }
        }
        free(line);
        line = NULL;
    }

    for (j = 0; j < lines; j++) {
        free(array[j]);
    }
    free(array);

    if (matches[0] == false || matches[1] == false || matches[2] == false) {
        printf("No pudo encontrar lo solicitado en Biblia.txt!\n");
        return 1;
    }

    return 0;
}
