#include "file.h"

#include <stdio.h>
#include <stdlib.h>

const char* file_open(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "[ERROR] [FILE] Failed to open file!\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int file_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* string = (char*) malloc(file_length + 1); // +1 for '\0'
    if (!string) {
        fprintf(stderr, "[ERROR] [FILE] Failed to allocate memory for string!\n");
        fclose(file);
        return NULL;
    }

    unsigned int bytes_read = fread(string, 1, file_length, file);
    if (bytes_read != file_length) {
        fprintf(stderr, "[ERROR] [File] Failed to read entire file!\n");
        free(string);
        fclose(file);
        return NULL;
    }
    string[bytes_read] = '\0';

    fclose(file);

    printf("[INFO] [FILE] Loaded file into string!\n");
    return string;
}
