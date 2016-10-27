#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include <stdbool.h>
#include "main.h"
#include "readconfig.h"
#include "readfile.h"
#include "makexml.h"

#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

/*
 * This program is designed to take a text file and convert part of it into xml.
 */
int main(int argc, char **argv) {
    char *config_file = NULL;
    int status;
    int i;
    bool meaning = false;

    atexit(cleanup);

    if (argc == 1) {
        /* No arguments were passed */
        print_usage(0);
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
		if ((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) && config_file == NULL) {
            if (argc > i + 1) {
                i++;
                config_file = (char *) malloc((strlen(argv[i]) + 1) * sizeof(char *));
                strcpy(config_file, argv[i]);
                #ifdef DEBUG
                    printf("\targ %d: %s\n", i, argv[i]);
                #endif
            }
            else {
                if (config_file != NULL) {
                    free(config_file);
                }
                print_usage(1);
                return 1;
            }
		}
		else if ((strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--significado") == 0) && meaning == false) {
            meaning = true;
		}
		else {
            /* Incorrect usage */
            if (config_file != NULL) {
                free(config_file);
            }
			print_usage(1);
			return 1;
		}
	}

    if (meaning == true) {
        if (config_file != NULL) {
            free(config_file);
            config_file = NULL;
        }
        printf("El significado de la vida es: %d\n", MEANING);
        return 0;
    }

    config = (CONFIG *) malloc(sizeof(CONFIG));
    config->file = NULL;
    config->bible = NULL;
    config->book = NULL;
    config->chapter = NULL;
    config->chapter_numbers = NULL;

    status = read_config(config_file);
    if (status != 0) {
        free(config_file);
        return 1;
    }

    if (strcmp(config->file, argv[0]) == 0 ||
            strcmp(config->file, config_file) == 0 ||
            strcmp(config->file, "Biblia.txt") == 0 ||
            strcmp(config->file, "config.dtd") == 0 ||
            strcmp(config->file, "generarxml.dtd") == 0) {
        free(config_file);
        config_file = NULL;
        printf("Nombre de archivo de salida invalido!\n");
        return 1;
    }

    free(config_file);
    config_file = NULL;

    printf("Configuración:\n");
    printf("\tArchivo: %s\n", config->file);
    printf("\tBiblia: %s\n", config->bible);
    printf("\tLibro: %s\n", config->book);
    printf("\tNombre de capitulo: %s\n", config->chapter);
    printf("\tNumeros de capitulo: %s\n", config->chapter_numbers);

    book = (BOOK *) malloc(sizeof(BOOK));
    status = read_file();
    if (status != 0) {
        printf("Falló leer Biblia.txt!\n");
        return 1;
    }

    make_xml();

    printf("El achivo %s ha sido generado.\n", config->file);

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
    fprintf(stderr, "libxml2 no está configurado correctamente\n");
    return 1;
}

#endif

/*
 * This function is called on exit to clean up the memory usage.
 */
void cleanup() {
    /* cleanup on aisle 3 */
    int i;
    int j;

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

    if (book) {
        for (i = 0; i < book->chapters; i++) {
            for (j = 0; j < book->chapter[i]->verses; j++) {
                free(book->chapter[i]->verse[j]);
            }
            free(book->chapter[i]->verse);
            free(book->chapter[i]);
        }
        free(book->chapter);
        free(book);
    }
}

/*
 * Print information on the program's usage. If the argument is 1, the user
 * put something incorrect as an argument.
 */
void print_usage(int error) {
	if (error == 1) {
		printf("Opcion desconocido!\n\n");
	}

	printf("usage: generarxml [opciones] \n");
    printf("  -c, --config <archivo>    archivo de configuración\n");
    /*printf("  -s, --significado         imprimir el significado de la vida\n");*/
}
