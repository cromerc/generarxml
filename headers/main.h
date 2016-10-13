struct configuration {
    char *file;
    char *bible;
    char *book;
    char *chapter;
    char *chapter_numbers;
} typedef CONFIG;

CONFIG *config;

void cleanup();
void printusage(int error);
