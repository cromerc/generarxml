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
    size_t n = 0;
    ssize_t nchr = 0;
    size_t idx = 0;
    size_t it = 0;
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

    i = 0;

    file = fopen("Biblia.txt", "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    if (!(array = calloc(new_max, sizeof(*array)))) {
        fprintf(stderr, "allocación de memoria falló.");
        return 1;
    }

    while ((nchr = getline(&line, &n, file)) != -1) {
        while (nchr > 0 && (line[nchr - 1] == '\n' || line[nchr - 1] == '\r' || line[nchr - 1] == ' ' || line[nchr - 1] == '\t')) {
            /* remove whitespace from end of the line */
            line[nchr--] = 0;
        }
        line[nchr] = '\0';
        array[idx++] = strdup(line);

        /* not enough memory for more lines, time to allocate more memory */
        if (idx == new_max) {
            new_max = new_max * 2;
            char **tmp = realloc(array, new_max * sizeof(*array));
            if (!tmp) {
                fprintf(stderr, "reallocación de memoria falló.");
                return 1;
            }
            array = tmp;
        }
    }
    
    /* free the extra unused memory */
    if (new_max > idx) {
        char **tmp = realloc(array, idx * sizeof(*array));
        if (!tmp) {
            fprintf(stderr, "reallocación de memoria falló.");
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

    printf ("\nLines in file:\n\n");
    for (it = 0; it < 20; it++) {
        line = latin2utf8(array[it]);
        /* printf("  array [%lu]  %s\n", (long) it, line); */
        if (line != NULL) {
            if (strcmp(line, config->bible) == 0) {
                printf("Bible match: %d -> %s\n", (int) it + 1, line);
            }
            if (strcmp(line, config->book) == 0) {
                printf("Book match: %d -> %s\n", (int) it + 1, line);
            }
        }
        free(line);
    }

    for (it = 0; it < idx; it++) {
        free(array[it]);
    }
    free(array);

    return 0;
}
