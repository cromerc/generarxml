struct configuration {
    xmlChar *file;
    xmlChar *bible;
    xmlChar *book;
    xmlChar *chapter;
    xmlChar *chapter_numbers;
} typedef CONFIG;

CONFIG *config;

void cleanup();
void printusage(int error);
