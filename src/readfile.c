#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "main.h"
#include "readfile.h"
#include "encoding.h"
#include "string.h"

int readfile(CONFIG *config) {
    FILE *file = NULL;
    int start;
    int end;
    int i = 0;
    char *line = NULL;
    char **array = NULL;
    ssize_t chars = 0;
    size_t lines = 0;
    size_t j = 0;
    size_t new_max = MAX_LINES;

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

    if (end != 0 && start > end) {
        printf("Archivo de configuración invalido!");
        return 1;
    }

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
        lines++;
        array[lines] = strdup(line);

        /* not enough memory for more lines, time to allocate more memory */
        if (lines == new_max) {
            new_max = new_max * 2;
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
        line = latin2utf8(array[j]);
        /* printf("  array [%lu]  %s\n", (long) j, line); */
        if (line != NULL) {
            if (strcmp(line, config->bible) == 0) {
                #ifdef DEBUG
                    printf("Bible match: %lu -> %s\n", (long) j, line);
                #endif
            }
            if (strcmp(line, config->book) == 0) {
                #ifdef DEBUG
                    printf("Book match: %lu -> %s\n", (long) j, line);
                #endif
            }
        }
        free(line);
    }

    for (j = 0; j < lines; j++) {
        free(array[j]);
    }
    free(array);

    return 0;
}
