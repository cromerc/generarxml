#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "main.h"

/*
 * This function reads the designated config file and save into int the
 * config struct.
 */
int readconfig(char *config_file) {
    xmlParserCtxtPtr context;
    xmlDocPtr config_xml = NULL;
    xmlNodePtr root = NULL;
    xmlNodePtr node = NULL;
    xmlNodePtr subnode = NULL;

    /* Initilize the library */
    LIBXML_TEST_VERSION
    
    context = xmlNewParserCtxt();
    if (context == NULL) {
        fprintf(stderr, "No pudo alocar contexto de analizador!\n");
        return 1;
    }

    config_xml = xmlCtxtReadFile(context, config_file, "UTF-8", XML_PARSE_DTDVALID);
    if (config_xml == NULL) {
        fprintf(stderr, "Falló analizar %s\n", config_file);
        xmlFreeParserCtxt(context);
        return 1;
    }
    else {
        if (context->valid == 0) {
            fprintf(stderr, "Falló validar %s\n", config_file);
            xmlFreeParserCtxt(context);
            return 1;
        }

        root = xmlDocGetRootElement(config_xml);

        if (root == NULL) {
            fprintf(stderr,"El archivo %s no contiene xml\n", config_file);
            xmlFreeDoc(config_xml);
            return 1;
        }

        /* Run through the nodes to find the config information. */
        node = root->xmlChildrenNode;
        while (node != NULL) {
            if ((!xmlStrcmp(node->name, (const xmlChar *) "output"))){
                xmlChar *file = xmlNodeListGetString(config_xml, node->xmlChildrenNode, 1);
                config->file = (char *) malloc(strlen((char *) file) + 1 * sizeof(char));
                if (!config->file) {
                    fprintf(stderr, "Allocación de memoria falló.");
                    return 1;
                }
                strcpy(config->file, (char *) file);
                xmlFree(file);
            }
            else if ((!xmlStrcmp(node->name, (const xmlChar *) "bible"))){
                xmlChar *bible = xmlNodeListGetRawString(config_xml, node->xmlChildrenNode, 1);
                config->bible = (char *) malloc(strlen((char *) bible) + 1 * sizeof(char));
                if (!config->bible) {
                    fprintf(stderr, "Allocación de memoria falló.");
                    return 1;
                }
                strcpy(config->bible, (char *) bible);
                xmlFree(bible);
            }
            else if ((!xmlStrcmp(node->name, (const xmlChar *) "book"))){
                xmlChar *book = xmlNodeListGetString(config_xml, node->xmlChildrenNode, 1);
                config->book = (char *) malloc(strlen((char *) book) + 1 * sizeof(char));
                if (!config->book) {
                    fprintf(stderr, "Allocación de memoria falló.");
                    return 1;
                }
                strcpy(config->book, (char *) book);
                xmlFree(book);
            }
            else if ((!xmlStrcmp(node->name, (const xmlChar *) "chapter"))){
                subnode = node->xmlChildrenNode;
                while (subnode != NULL) {
                    if ((!xmlStrcmp(subnode->name, (const xmlChar *) "name"))){
                        xmlChar *chapter = xmlNodeListGetString(config_xml, subnode->xmlChildrenNode, 1);
                        config->chapter = (char *) malloc(strlen((char *) chapter) + 1 * sizeof(char));
                        if (!config->chapter) {
                            fprintf(stderr, "Allocación de memoria falló.");
                            return 1;
                        }
                        strcpy(config->chapter, (char *) chapter);
                        xmlFree(chapter);
                    }
                    if ((!xmlStrcmp(subnode->name, (const xmlChar *) "number"))){
                        xmlChar *chapter_numbers = xmlNodeListGetString(config_xml, subnode->xmlChildrenNode, 1);
                        config->chapter_numbers = (char *) malloc(strlen((char *) chapter_numbers) + 1 * sizeof(char));
                        if (!config->chapter_numbers) {
                            fprintf(stderr, "Allocación de memoria falló.");
                            return 1;
                        }
                        strcpy(config->chapter_numbers, (char *) chapter_numbers);
                        xmlFree(chapter_numbers);
                    }

                    subnode = subnode->next;
                }
            }
            
            node = node->next;
        }
        
        xmlFreeDoc(config_xml);
    }

    /* If any config info is missing, abort */
    if (config->file == NULL || config->bible == NULL || config->book == NULL || config->chapter == NULL || config->chapter_numbers == NULL) {
        printf("El archivo de configuración es invalido!\n");
        xmlFreeParserCtxt(context);
        return 1;
    }
    
    xmlFreeParserCtxt(context);

    return 0;
}
