#define MEANING 30 + 12

/*
 * This struct houses all the information from the .conf file.
 *
 * file is the output xml file
 * bible is the bible we are searching for
 * book is the book we are searching for
 * chapter contains the name of the chapter
 * chapter_numbers contains the specific chapter numbers to search for
 */
struct configuration {
    char *file;
    char *bible;
    char *book;
    char *chapter;
    char *chapter_numbers;
} typedef CONFIG;

/*
 * This struct houses all the data for a specific chapter.
 *
 * chapter is the chapter number of the struct
 * current is used to track iteration in the struct
 * verses is the number of verses contained in the verse variable
 * verse contains the verses
 */
struct chapterdata {
    int chapter;
    int current;
    int verses;
    char **verse;
} typedef CHAPTER;

/*
 * This struct contains the data for the book and it's chapters.
 * current is used for iteration in the struct
 * chapters is the ammount of chapters that are contained in the chapter variable
 * chapter contains all of the chapters
 */
struct bookdata {
    int current;
    int chapters;
    CHAPTER **chapter;
} typedef BOOK;

/*
 * Global variables.
 *
 * config contains the configuration that has been loaded from the .conf file
 * book contains all the information read from Biblia.txt
 */
CONFIG *config;
BOOK *book;

/*
 * This function is called on exit to clean up the memory usage.
 */
void cleanup();

/*
 * Print information on the program's usage. If the argument is 1, the user
 * put something incorrect as an argument.
 */
void printusage(int error);
