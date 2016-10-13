#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>

#ifdef LIBXML_TREE_ENABLED

static void
print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }

        print_element_names(cur_node->children);
    }
}

int readconfig(char *config_file) {
    /* Initilize the library */
    LIBXML_TEST_VERSION
    
    xmlParserCtxtPtr context;
    xmlDocPtr config = NULL;
    xmlNodePtr root_element = NULL;
    
    context = xmlNewParserCtxt();
    if (context == NULL) {
        fprintf(stderr, "No pudo alocar contexto de analizador!\n");
        return 1;
    }

    config = xmlCtxtReadFile(context, config_file, NULL, XML_PARSE_DTDVALID);
    if (config == NULL) {
        fprintf(stderr, "Falló analizar %s\n", config_file);
    }
    else {
        if (context->valid == 0) {
            fprintf(stderr, "Falló validar %s\n", config_file);
        }

        root_element = xmlDocGetRootElement(config);
        print_element_names(root_element);
        
        xmlFreeDoc(config);
    }
    xmlFreeParserCtxt(context);

    return 0;
}

#else

int readconfig(char *config_file) {
    fprintf(stderr, "libxml2 no tiene tree support compilado\n");
    return 1;
}

#endif
