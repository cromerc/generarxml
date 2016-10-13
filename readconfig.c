#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include "main.h"

int readconfig(char *config_file, CONFIG *config) {
    /* Initilize the library */
    LIBXML_TEST_VERSION
    
    xmlParserCtxt *context;
    xmlDoc *config_xml = NULL;
    xmlNode *root = NULL;
    xmlNode *node = NULL;
    xmlNode *subnode = NULL;
    
    context = xmlNewParserCtxt();
    if (context == NULL) {
        fprintf(stderr, "No pudo alocar contexto de analizador!\n");
        return 1;
    }

    config_xml = xmlCtxtReadFile(context, config_file, NULL, XML_PARSE_DTDVALID);
    if (config_xml == NULL) {
        fprintf(stderr, "Falló analizar %s\n", config_file);
    }
    else {
        if (context->valid == 0) {
            fprintf(stderr, "Falló validar %s\n", config_file);
        }

        root = xmlDocGetRootElement(config_xml);

        node = root->xmlChildrenNode;
        while (node != NULL) {
            if ((!xmlStrcmp(node->name, (const xmlChar *) "output"))){
                config->file = xmlNodeListGetString(config_xml, node->xmlChildrenNode, 1);
            }
            else if ((!xmlStrcmp(node->name, (const xmlChar *) "bible"))){
                config->bible = xmlNodeListGetString(config_xml, node->xmlChildrenNode, 1);
            }
            else if ((!xmlStrcmp(node->name, (const xmlChar *) "book"))){
                config->book = xmlNodeListGetString(config_xml, node->xmlChildrenNode, 1);
            }
            else if ((!xmlStrcmp(node->name, (const xmlChar *) "chapter"))){
                subnode = node->xmlChildrenNode;
                while (subnode != NULL) {
                    if ((!xmlStrcmp(subnode->name, (const xmlChar *) "name"))){
                        config->chapter = xmlNodeListGetString(config_xml, subnode->xmlChildrenNode, 1);
                    }
                    if ((!xmlStrcmp(subnode->name, (const xmlChar *) "number"))){
                        config->chapter_numbers = xmlNodeListGetString(config_xml, subnode->xmlChildrenNode, 1);
                    }

                    subnode = subnode->next;
                }
            }
            
            node = node->next;
        }
    
        xmlFreeDoc(config_xml);
    }

    if (config->file == NULL || config->bible == NULL || config->book == NULL || config->chapter == NULL || config->chapter_numbers == NULL) {
        printf("El archivo de configuración es invalido!");
        return 1;
    }
    
    xmlFreeParserCtxt(context);

    return 0;
}
