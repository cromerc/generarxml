#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

/*
 * This function converts the latin1 ISO_8859-1 encoding into utf-8.
 */
char *latin_to_utf8(char *input) {
    if (input && strlen(input) > 0) {
        iconv_t cd = iconv_open("UTF-8", "ISO_8859-1");

        char temp[strlen(input)];
        memcpy(temp, input, strlen(input) + 1);

        char *in_buffer = &temp[0];
        size_t in_left = sizeof(temp);

        /* The more tildes and ñs that the string has, the longer the size needs to be.
         * if the string is 100% of the aforementioned symbols, we will need a string of
         * double the size.
         */
        char output[strlen(temp) * 2];
        char *out_buffer = &output[0];
        size_t out_left = sizeof(output);

        do {
            if (iconv(cd, &in_buffer, &in_left, &out_buffer, &out_left) == (size_t) -1) {
                #ifdef DEBUG
                    printf("%s\n", input);
                    perror("iconv");
                #endif
                iconv_close(cd);
                return NULL;
            }
        }
        while (in_left > 0 && out_left > 0);
        *out_buffer = 0;

        #if defined(DEBUG) && defined(DEBUG_ENCODING)
            printf("latin to UTF-8: input: %s\n", input);
            printf("latin to UTF-8: output: %s\n", output);
        #endif

        char *temp2 = (char *) malloc((strlen(output) + 1) * sizeof(char));
        memcpy(temp2, output, strlen(output) + 1);

        iconv_close(cd);

        return temp2;
    }
    return NULL;
}

/*
 * This function converts the utf-8 encoding into latin1 ISO_8859-1.
 */
char *utf8_to_latin(char *input) {
    if (input && strlen(input) > 0) {
        iconv_t cd = iconv_open("ISO_8859-1", "UTF-8");

        char temp[strlen(input)];
        memcpy(temp, input, strlen(input) + 1);

        char *in_buffer = &temp[0];
        size_t in_left = sizeof(temp);

        /* The more tildes and ñs that the string has, the longer the size needs to be.
         * if the string is 100% of the aforementioned symbols, we will need a string of
         * double the size.
         */
        char output[strlen(temp) * 2];
        char *out_buffer = &output[0];
        size_t out_left = sizeof(output);

        do {
            if (iconv(cd, &in_buffer, &in_left, &out_buffer, &out_left) == (size_t) -1) {
                #ifdef DEBUG
                    printf("%s\n", input);
                    perror("iconv");
                #endif
                iconv_close(cd);
                return NULL;
            }
        }
        while (in_left > 0 && out_left > 0);
        *out_buffer = 0;

        #if defined(DEBUG) && defined(DEBUG_ENCODING)
            printf("UTF-8 to latin: input: %s\n", input);
            printf("UTF-8 to latin: output: %s\n", output);
        #endif

        char *temp2 = (char *) malloc((strlen(output) + 1) * sizeof(char));
        memcpy(temp2, output, strlen(output) + 1);

        iconv_close(cd);

        return temp2;
    }
    return NULL;
}
