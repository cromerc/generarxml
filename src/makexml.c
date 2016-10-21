#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "main.h"

/*
 * This function generates the xml and saves it to the output file.
 */
void makexml() {
    int i = 0;
    int j = 0;
    int length;
    char *temp = NULL;
    CHAPTER *chapter = NULL;
    xmlDocPtr doc = NULL;
    xmlNodePtr bibletag = NULL;
    xmlNodePtr booktag  = NULL;
    xmlNodePtr chaptertag = NULL;
    xmlNodePtr versetag = NULL;
    xmlNodePtr node = NULL;
    xmlNodePtr text = NULL;

    LIBXML_TEST_VERSION;

    doc = xmlNewDoc(BAD_CAST "1.0");
    bibletag = xmlNewNode(NULL, BAD_CAST "Biblia");
    xmlDocSetRootElement(doc, bibletag);

    /* add attribute to bible tag */
    xmlNewProp(bibletag, BAD_CAST "nombre", BAD_CAST config->bible);

    /* add dtd */
    xmlCreateIntSubset(doc, BAD_CAST "Biblia", NULL, BAD_CAST "generarxml.dtd");

    booktag = xmlNewNode(NULL, BAD_CAST "Libro");
    xmlAddChild(bibletag, booktag);

    node = xmlNewNode(NULL, BAD_CAST "Nombre");
    text = xmlNewText(BAD_CAST config->book);
    xmlAddChild(node, text);
    xmlAddChild(booktag, node);

    /* add the chapters */
    for (i = 0; i < book->chapters; i++) {
        chaptertag = xmlNewNode(NULL, BAD_CAST "Capitulo");
        xmlAddChild(booktag, chaptertag);

        length = snprintf(NULL, 0, "%d", book->chapter[i]->chapter) + strlen(config->chapter);
        temp = (char *) malloc((length + 2) * sizeof(char));
        snprintf(temp, length + 2, "%s %d", config->chapter, book->chapter[i]->chapter);

        node = xmlNewNode(NULL, BAD_CAST "Nombre");
        text = xmlNewText(BAD_CAST temp);
        xmlAddChild(node, text);
        xmlAddChild(chaptertag, node);

        free(temp);

        /* add the verses */
        chapter = book->chapter[i];
        chapter->current = 0;
        for (chapter->current = 0; chapter->current < chapter->verses; chapter->current++) {
            versetag = xmlNewNode(NULL, BAD_CAST "Versiculo");
            xmlAddChild(chaptertag, versetag);

            length = snprintf(NULL, 0, "%d", chapter->current + 1) + strlen("VERSICULO");
            temp = (char *) malloc((length + 2) * sizeof(char));
            snprintf(temp, length + 2, "VERSICULO %d", chapter->current + 1);

            node = xmlNewNode(NULL, BAD_CAST "Nombre");
            text = xmlNewText(BAD_CAST temp);
            xmlAddChild(node, text);
            xmlAddChild(versetag, node);

            free(temp);

            node = xmlNewNode(NULL, BAD_CAST "Descripcion");
            text = xmlNewText(BAD_CAST chapter->verse[chapter->current]);
            xmlAddChild(node, text);
            xmlAddChild(versetag, node);
        }
    }

    #ifdef DEBUG
        printf("XML output:\n");
        xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
    #endif

    xmlSaveFormatFileEnc(config->file, doc, "UTF-8", 1);

    /* cleanup on aisle 3 */
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
}
