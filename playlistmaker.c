#include "playlistmaker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copy_csv_line() {
    char details_file_path[] = "details.csv";
    char new_file_name[50];
    char new_file_path[100];
    char playlist_file_path[] = "playlistnames.csv";
    FILE *details_file, *new_file, *playlist_file;
    char line[1024];
    int current_line_number = 1, line_number;

    // Open the details.csv file
    details_file = fopen(details_file_path, "r");
    if (details_file == NULL) {
        printf("Error opening file: %s\n", details_file_path);
        exit(1);
    }

    // Ask the user for the line number
    printf("Enter the number corresponding to the song to be added to the playlist: ");
    scanf("%d", &line_number);

    // Read the details.csv file line by line
    while (fgets(line, 1024, details_file)) {
        // If the current line number matches the user input line number
        if (current_line_number == line_number) {
            // Ask the user for the name of the new file
            printf("Enter the name of the new file: ");
            scanf("%s", new_file_name);

            // Create the new file path
            sprintf(new_file_path, "%s.csv", new_file_name);

            // Open the new file
            new_file = fopen(new_file_path, "w");
            if (new_file == NULL) {
                printf("Error creating file: %s\n", new_file_path);
                exit(1);
            }

            // Write the current line to the new file
            fputs(line, new_file);

            // Close the new file
            fclose(new_file);

            // Open the playlistnames.csv file in append mode
            playlist_file = fopen(playlist_file_path, "a");
            if (playlist_file == NULL) {
                printf("Error opening file: %s\n", playlist_file_path);
                exit(1);
            }

            // Append the new file name to the playlistnames.csv file
            fprintf(playlist_file, "%s\n", new_file_path);

            // Close the playlistnames.csv file
            fclose(playlist_file);

            // Exit the loop
            break;
        }

        // Increment the current line number
        current_line_number++;
    }

    // Close the details.csv file
    fclose(details_file);
}