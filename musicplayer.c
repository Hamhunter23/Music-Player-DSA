#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include "songSearcher.h"
#include "playlistmaker.h"

#define MAX_SONGS 50
#define MAX_TITLE_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define MAX_LINE_LENGTH 1024

// Structure to hold information about a song
typedef struct {
    char title[MAX_TITLE_LENGTH];
    char artist[MAX_TITLE_LENGTH];
    char duration[MAX_TITLE_LENGTH];
    char filename[MAX_FILENAME_LENGTH];
} Song;

// Structure to manage the music player
typedef struct {
    Song playlist[MAX_SONGS];
    int numSongs;
    int currentSongIndex;
    int isPlaying; // 1 if playing, 0 if paused
} MusicPlayer;

// Function to add a song to the playlist
void addSong(MusicPlayer *player, const char *title, const char *artist, const char *duration, const char *filename) {
    if (player->numSongs < MAX_SONGS) {
        strcpy(player->playlist[player->numSongs].title, title);
        strcpy(player->playlist[player->numSongs].artist, artist);
        strcpy(player->playlist[player->numSongs].duration, duration);
        strcpy(player->playlist[player->numSongs].filename, filename);
        player->numSongs++;
    } else {
        printf("Playlist is full.\n");
    }
}

// Function to display the playlist
void displayPlaylist(const MusicPlayer *player) {
    printf("Current Playlist:\n");
    for (int i = 0; i < player->numSongs; i++) {
        printf("%d. %s by %s (%s)\n", i + 1, player->playlist[i].title, player->playlist[i].artist, player->playlist[i].duration);
    }
}

// Function to play or resume a song
void playOrResumeSong(const char *filename) {
    printf("Playing: %s\n", filename);
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}

// Function to pause a song
void pauseSong() {
    printf("Pausing the song.\n");
    PlaySound(NULL, NULL, 0); // Stop playback
}

FILE* changePlaylist(FILE *oldFile){
    if (oldFile != NULL) {
        fclose(oldFile);
    }
    FILE *file = fopen("playlistnames.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }
    char line[256]; // Adjust the size as needed
    int lineNumber = 1;
    while (fgets(line, sizeof(line), file)) {
        printf("%d. %s", lineNumber, line);
        lineNumber++;
    }
    int selectedLine;
    printf("\nSelect a playlist ");
    scanf("%d", &selectedLine);
    selectedLine = selectedLine + 1;
    if (selectedLine < 1 || selectedLine - 1 >= lineNumber) {
        printf("Invalid line number.\n");
        return NULL;
    }
    fseek(file, 0, SEEK_SET);
    for (int i = 1; i < selectedLine; i++) {
        fgets(line, sizeof(line), file);
    }
    line[strcspn(line, "\r\n")] = 0; // Remove newline character
    printf("Selected playlist: %s\n", line);
    FILE* playlistFile = fopen(line, "r");
    if (playlistFile == NULL) {
        printf("Error opening playlist file.\n");
        return NULL;
    }
    return playlistFile;
}
int main() {
    MusicPlayer player;
    player.numSongs = 0;
    player.currentSongIndex = -1; // No song playing initially
    player.isPlaying = 0; // 0 indicates paused, 1 indicates playing

    // Read from CSV file
    FILE *file = changePlaylist();

    char line[256]; // Adjust the size as needed
    while (fgets(line, sizeof(line), file)) {
        char title[MAX_TITLE_LENGTH], artist[MAX_TITLE_LENGTH], duration[MAX_TITLE_LENGTH], filename[MAX_FILENAME_LENGTH];
        if (sscanf(line, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"", title, artist, duration, filename) == 4) {
            addSong(&player, title, artist, duration, filename);
        } else {
            printf("Error reading line from file.\n");
        }
    }
    while (1) {
        int choice;
        printf("\nMenu:\n1. Display Playlist\n2. Play/Resume Song\n3. Pause Song\n4. Next Song\n5. Previous Song\n6. Create a Playlist\n7. Change Playlist\n8. Search for songs\n9. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayPlaylist(&player);
                break;
            case 2:
                if (player.isPlaying) {
                    pauseSong();
                    player.isPlaying = 0;
                } else {
                    int songIndex;
                    printf("Enter song number to play: ");
                    scanf("%d", &songIndex);
                    if (songIndex > 0 && songIndex <= player.numSongs) {
                        player.currentSongIndex = songIndex - 1;
                        playOrResumeSong(player.playlist[player.currentSongIndex].filename);
                        player.isPlaying = 1;
                    } else {
                        printf("Invalid song number.\n");
                    }
                }
                break;
            case 3:
                if (player.isPlaying) {
                    pauseSong();
                    player.isPlaying = 0;
                } else {
                    printf("No song is currently playing.\n");
                }
                break;
            case 4:
                // Next Song
                player.currentSongIndex = (player.currentSongIndex + 1) % player.numSongs;
                playOrResumeSong(player.playlist[player.currentSongIndex].filename);
                player.isPlaying = 1;
                break;
            case 5:
                // Previous Song
                player.currentSongIndex = (player.currentSongIndex - 1 + player.numSongs) % player.numSongs;
                playOrResumeSong(player.playlist[player.currentSongIndex].filename);
                player.isPlaying = 1;
                break;
            case 6:
                copy_csv_line();
                break;
            case 7:
                file = changePlaylist();
                break;
            case 8:
                searchInCSV();
                break;
            case 9:
                printf("Exiting the music player.\n");
                fclose(file);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
