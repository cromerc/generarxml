#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include "main.h"
#include "readconfig.h"
#include "readfile.h"

#ifdef LIBXML_TREE_ENABLED

/*
 * This program is designed to take a text file and convert part of it into xml.
 */
int main(int argc, char **argv) {
    char *config_file = NULL;
    int status;
    int i;

    atexit(cleanup);

    if (argc == 1) {
        /* No arguments were passed */
        printusage(0);
        return 1;
    }
    
    /* Read the command line arguments */
    #ifdef DEBUG
        printf("Arguments:\n");
    #endif
	for (i = 1; i < argc; i++) {
        #ifdef DEBUG
            printf("\targ %d: %s\n", i, argv[i]);
        #endif
		if ((strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--config") == 0) && config_file == NULL) {
            if (argc > i + 1) {
                i++;
                config_file = (char *) malloc((strlen(argv[2]) + 1) * sizeof(char *));
                strcpy(config_file, argv[2]);
                #ifdef DEBUG
                    printf("\targ %d: %s\n", i, argv[i]);
                #endif
            }
            else {
                if (config_file != NULL) {
                    free(config_file);
                }
                printusage(1);
                return 1;
            }
		}
		else {
            /* Incorrect usage */
            if (config_file != NULL) {
                free(config_file);
            }
			printusage(1);
			return 1;
		}
	}

    config = (CONFIG *) malloc(sizeof(CONFIG));
    config->file = NULL;
    config->bible = NULL;
    config->book = NULL;
    config->chapter = NULL;
    config->chapter_numbers = NULL;

    status = readconfig(config_file, config);
    free(config_file);
    config_file = NULL;
    if (status != 0) {
        return 1;
    }

    printf("Configuración:\n");
    printf("\tArchivo: %s\n", config->file);
    printf("\tBiblia: %s\n", config->bible);
    printf("\tLibro: %s\n", config->book);
    printf("\tNombre de capitulo: %s\n", config->chapter);
    printf("\tNumeros de capitulo: %s\n", config->chapter_numbers);

    status = readfile(config);
    if (status != 0) {
        printf("Falló leer Biblia.txt!\n");
        return 1;
    }

    free(config->file);
    free(config->bible);
    free(config->book);
    free(config->chapter);
    free(config->chapter_numbers);
    free(config);
    config = NULL;

    return 0;
}

#else

/*
 * Alternate main designed to prevent problems if the host system does not have
 * tree support enabled during compile.
 */
int main(int argc, char *argv[]) {
    fprintf(stderr, "libxml2 no tiene tree support compilado\n");
    return 1;
}

#endif

/*
 * This function is called on exit to clean up the memory usage.
 */
void cleanup() {
    /* Cleanup on aisle 3 */
    if (config) {
        if (config->file) {
            free(config->file);
        }
        if (config->bible) {
            free(config->bible);
        }
        if (config->book) {
            free(config->book);
        }
        if (config->chapter) {
            free(config->chapter);
        }
        if (config->chapter_numbers) {
            free(config->chapter_numbers);
        }

        free(config);
    }
}

/*
 * Print information on the program's usage. If the argument is 1, the user
 * put something incorrect as an argument.
 */
void printusage(int error) {
	if (error == 1) {
		printf("Opcion desconocido!\n\n");
	}

	printf("usage: generarxml [opciones] \n");
    printf("  -c, --config <archivo>    archivo de configuración\n");
}
