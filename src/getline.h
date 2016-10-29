#ifdef __MINGW32__
#define GET_LINE_MAX_MEM 128

size_t getline(char **lineptr, size_t *n, FILE *stream);
size_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream);
#endif
