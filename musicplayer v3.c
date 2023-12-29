#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>

#define MAX_TITLE_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define MAX_PLAYLIST_NAME_LENGTH 50
#define MAX_LINE_LENGTH 256

// Structure to hold information about a song
typedef struct SongNode {
    char title[MAX_TITLE_LENGTH];
    char filename[MAX_FILENAME_LENGTH];
    struct SongNode* next;
} SongNode;

// Structure to manage the music player
typedef struct {
    SongNode* playlistHead;
    SongNode* playlistTail;
    SongNode* currentSong; // Pointer to the currently playing song node
    BOOL isPaused;         // Flag to track pause state
} MusicPlayer;

// Function to add a song to the playlist
void addSong(MusicPlayer* player, const char* title, const char* filename) {
    SongNode* newSong = (SongNode*)malloc(sizeof(SongNode));
    if (newSong == NULL) {
        printf("Memory allocation error.\n");
        exit(1);
    }

    strcpy(newSong->title, title);
    strcpy(newSong->filename, filename);
    newSong->next = NULL;

    if (player->playlistTail == NULL) {
        // The playlist is empty
        player->playlistHead = player->playlistTail = newSong;
    } else {
        player->playlistTail->next = newSong;
        player->playlistTail = newSong;
    }
}

// Function to display the playlist
void displayPlaylist(const MusicPlayer* player) {
    printf("Current Playlist:\n");
    int i = 1;
    SongNode* current = player->playlistHead;
    while (current != NULL) {
        printf("%d. %s\n", i, current->title);
        current = current->next;
        i++;
    }
}

// Function to play a song
void playSong(const char* filename, BOOL* isPaused) {
    printf("Playing: %s\n", filename);
    if (*isPaused) {
        PlaySound(NULL, 0, 0); // Stop the currently playing sound (if paused)
        *isPaused = FALSE;
    }
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}

// Function to pause the current song
void pauseSong(BOOL* isPaused) {
    printf("Pausing...\n");
    *isPaused = TRUE;
    PlaySound(NULL, 0, 0); // Stop the currently playing sound
}

// Function to resume playback
void resumeSong(const char* filename, BOOL* isPaused) {
    printf("Resuming...\n");
    *isPaused = FALSE;
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}

// Function to read songs from a given playlist CSV file and populate the playlist
void readSongsFromPlaylistCSV(MusicPlayer* player, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    // Skip the header line
    fgets(line, MAX_LINE_LENGTH, file);

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char title[MAX_TITLE_LENGTH];
        char filename[MAX_FILENAME_LENGTH];

        if (sscanf(line, "%[^,],%s", title, filename) == 2) {
            addSong(player, title, filename);
        } else {
            printf("Invalid line format: %s\n", line);
        }
    }

    fclose(file);
}
void createNewPlaylist(MusicPlayer *player) {
    char newPlaylistName[MAX_PLAYLIST_NAME_LENGTH];
    printf("Enter the name for the new playlist file (without extension): ");
    scanf("%s", newPlaylistName);
 
    char newPlaylistFilename[MAX_FILENAME_LENGTH];
    sprintf(newPlaylistFilename, "%s.csv", newPlaylistName);
 
    FILE *newPlaylistFile = fopen(newPlaylistFilename, "w");
    if (newPlaylistFile == NULL) {
        printf("Error creating the new playlist file.\n");
        return;
    }
 
    fprintf(newPlaylistFile, "Song Title,Filename\n");
    for (int i = 0; i < player->numSongs; i++) {
        printf("Include '%s' in the new playlist? (1 for Yes / 0 for No): ", player->playlist[i].title);
        int include;
        scanf("%d", &include);
 
        if (include == 1) {
            fprintf(newPlaylistFile, "%s,%s\n", player->playlist[i].title, player->playlist[i].filename);
        }
    }
 
    fclose(newPlaylistFile);
    printf("New playlist '%s' created successfully.\n", newPlaylistFilename);
}