#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include "main.h"
#include "readconfig.h"

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

    printf("file: %s\n", config->file);
    printf("bible: %s\n", config->bible);

    return 0;
}

/* Cleanup on aisle 3
 * */
void cleanup() {
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

/* Print how to use the program
 * */
void printusage(int error) {
	if (error == 1) {
		printf("Opcion desconocido!\n\n");
	}

	printf("usage: generarxml [opciones] \n");
    printf("  -s, --config <archivo>    archivo de configuración\n");
}
