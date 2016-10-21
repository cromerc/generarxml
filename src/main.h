#define MEANING 30 + 12

struct configuration {
    char *file;
    char *bible;
    char *book;
    char *chapter;
    char *chapter_numbers;
} typedef CONFIG;

struct chapterdata {
    int chapter;
    int current;
    int verses;
    char **verse;
} typedef CHAPTER;

struct bookdata {
    int current;
    int chapters;
    CHAPTER **chapter;
} typedef BOOK;

CONFIG *config;
BOOK *book;

void cleanup();
void printusage(int error);
