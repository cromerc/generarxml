#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include "main.h"
#include "readconfig.h"

#ifdef LIBXML_TREE_ENABLED

/*
 * This program is designed to take a text file and convert part of it into xml.
 */
int main(int argc, char *argv[]) {
    atexit(cleanup);
    
    char *config_file = "";

    if (argc == 1) {
        /* No arguments were passed */
        printusage(0);
        return 1;
    }
    
    /* Read the command line arguments */
    int i;
	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) && i != argc - 1 && strlen(config_file) == 0) {
			config_file = (char *) malloc(strlen(argv[i + 1]) * sizeof(char));
			config_file = argv[i + 1];
            i++;
		}
		else {
            /* Incorrect usage or used the same option more than once */
			printusage(1);
			return 1;
		}
	}

    config = (CONFIG *) malloc (sizeof(CONFIG));

    int status = readconfig(config_file, config);
    if (status != 0) {
        return 1;
    }

    printf("Configuración:\n");
    printf("Archivo: %s\n", config->file);
    printf("Biblia: %s\n", config->bible);
    printf("Libro: %s\n", config->book);
    printf("Nombre de capitulo: %s\n", config->chapter);
    printf("Numeros de capitulo: %s\n", config->chapter_numbers);

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
    if (config != NULL) {
        if (config->file != NULL) {
            xmlFree(config->file);
        }
        if (config->bible != NULL) {
            xmlFree(config->bible);
        }
        if (config->book != NULL) {
            xmlFree(config->book);
        }
        if (config->chapter != NULL) {
            xmlFree(config->chapter);
        }
        if (config->chapter_numbers != NULL) {
            xmlFree(config->chapter_numbers);
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
    printf("  -s, --config <archivo>    archivo de configuración\n");
}
