#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "main.h"

/*
 * This function generates the xml and saves it to the output file.
 */
void make_xml() {
    int i = 0;
    int j = 0;
    int length;
    char *temp = NULL;
    CHAPTER *chapter = NULL;
    xmlDocPtr doc = NULL;
    xmlNodePtr bible_tag = NULL;
    xmlNodePtr book_tag  = NULL;
    xmlNodePtr chapter_tag = NULL;
    xmlNodePtr verse_tag = NULL;
    xmlNodePtr node = NULL;
    xmlNodePtr text = NULL;

    LIBXML_TEST_VERSION;

    doc = xmlNewDoc(BAD_CAST "1.0");
    bible_tag = xmlNewNode(NULL, BAD_CAST "Biblia");
    xmlDocSetRootElement(doc, bible_tag);

    /* add attribute to bible tag */
    xmlNewProp(bible_tag, BAD_CAST "nombre", BAD_CAST config->bible);

    /* add dtd */
    xmlCreateIntSubset(doc, BAD_CAST "Biblia", NULL, BAD_CAST "generarxml.dtd");

    book_tag = xmlNewNode(NULL, BAD_CAST "Libro");
    xmlAddChild(bible_tag, book_tag);

    node = xmlNewNode(NULL, BAD_CAST "Nombre");
    text = xmlNewText(BAD_CAST config->book);
    xmlAddChild(node, text);
    xmlAddChild(book_tag, node);

    /* add the chapters */
    for (i = 0; i < book->chapters; i++) {
        chapter_tag = xmlNewNode(NULL, BAD_CAST "Capitulo");
        xmlAddChild(book_tag, chapter_tag);

        length = snprintf(NULL, 0, "%d", book->chapter[i]->chapter) + strlen(config->chapter);
        temp = (char *) malloc((length + 2) * sizeof(char));
        snprintf(temp, length + 2, "%s %d", config->chapter, book->chapter[i]->chapter);

        node = xmlNewNode(NULL, BAD_CAST "Nombre");
        text = xmlNewText(BAD_CAST temp);
        xmlAddChild(node, text);
        xmlAddChild(chapter_tag, node);

        free(temp);

        /* add the verses */
        chapter = book->chapter[i];
        chapter->current = 0;
        for (chapter->current = 0; chapter->current < chapter->verses; chapter->current++) {
            verse_tag = xmlNewNode(NULL, BAD_CAST "Versiculo");
            xmlAddChild(chapter_tag, verse_tag);

            length = snprintf(NULL, 0, "%d", chapter->current + 1) + strlen("VERSICULO");
            temp = (char *) malloc((length + 2) * sizeof(char));
            snprintf(temp, length + 2, "VERSICULO %d", chapter->current + 1);

            node = xmlNewNode(NULL, BAD_CAST "Nombre");
            text = xmlNewText(BAD_CAST temp);
            xmlAddChild(node, text);
            xmlAddChild(verse_tag, node);

            free(temp);

            node = xmlNewNode(NULL, BAD_CAST "Descripcion");
            text = xmlNewText(BAD_CAST chapter->verse[chapter->current]);
            xmlAddChild(node, text);
            xmlAddChild(verse_tag, node);
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
