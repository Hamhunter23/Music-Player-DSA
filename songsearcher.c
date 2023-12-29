#include "songsearcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void searchInCSV() {
    char searchStr[MAX_LINE_LENGTH];
    printf("Enter the string to search: ");
    if (fgets(searchStr, MAX_LINE_LENGTH, stdin) == NULL) {
        perror("Error reading input");
    }
    // Remove newline character from the end of the search string
    size_t searchStrLength = strlen(searchStr);
    if (searchStrLength > 0 && searchStr[searchStrLength - 1] == '\n') {
        searchStr[searchStrLength - 1] = '\0';
    }
    FILE *file = fopen("details.csv", "r");

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        lineNumber++;

        // Remove newline character from the end of the line
        size_t lineLength = strlen(line);
        if (lineLength > 0 && line[lineLength - 1] == '\n') {
            line[lineLength - 1] = '\0';
        }

        // Search for the string in the line
        if (strstr(line, searchStr) != NULL) {
            printf("Match found at line %d: %s\n", lineNumber, line);
        }
    }

    fclose(file);
}