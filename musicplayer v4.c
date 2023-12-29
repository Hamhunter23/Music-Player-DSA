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
    int numSongs;          // Number of songs in the playlist
    int currentSongIndex;  // Index of the currently playing song in the playlist
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

    player->numSongs++;
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
    int numSongs;          // Number of songs in the playlist
    int currentIndex;      // Index of the currently playing song in the playlist
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

    player->numSongs++;
}

// Function to display the playlist
void displayPlaylist(const MusicPlayer* player) {
    printf("Current Playlist:\n");
    int i = 1;
    SongNode* current = player->playlistHead;
    while (current != NULL) {
        printf("%d. %s\n", i++, current->title);
        current = current->next;
    }
}

// Function to play the next song in the playlist
void playNextSong(MusicPlayer* player) {
    if (player->currentSong == NULL) {
        // No song is currently playing
        if (player->playlistHead != NULL) {
            // Start playing the first song in the playlist
            player->currentSong = player->playlistHead;
            player->currentIndex = 1;
            printf("Playing: %s\n", player->currentSong->title);
            PlaySound(player->currentSong->filename, NULL, SND_FILENAME | SND_ASYNC);
        } else {
            printf("The playlist is empty.\n");
        }
    } else {
        // Stop playing the current song
        printf("Stopping: %s\n", player->currentSong->title);
        PlaySound(NULL, NULL, 0);

        // Move to the next song in the playlist
        if (player->currentSong->next != NULL) {
            player->currentSong = player->currentSong->next;
            player->currentIndex++;
            printf("Playing: %s\n", player->currentSong->title);
            PlaySound(player->currentSong->filename, NULL, SND_FILENAME | SND_ASYNC);
        } else {
            printf("End of playlist.\n");
            player->currentSong = NULL;
            player->currentIndex = 0;
        }
    }
}

// Function to pause the currently playing song
void pauseSong(MusicPlayer* player) {
    if (player->currentSong != NULL) {
        if (!player->isPaused) {
            printf("Pausing: %s\n", player->currentSong->title);
            PlaySound(NULL, NULL, 0);
            player->isPaused = TRUE;
        } else {
            printf("Resuming: %s\n", player->currentSong->title);
            PlaySound(player->currentSong->filename, NULL, SND_FILENAME | SND_ASYNC);
            player->isPaused = FALSE;
        }
    } else {
        printf("No song is currently playing.\n");
    }
}

int main() {
    MusicPlayer player = { NULL, NULL, NULL, FALSE, 0, 0 };
    int choice;

    while (1) {
        printf("\nMusic Player Menu:\n");
        printf("1. Add a song to the playlist\n");
        printf("2. Display the playlist\n");
        printf("3. Play the next song\n");
        printf("4. Pause/Resume the currently playing song\n");
        printf("5. Stop the currently playing song\n");
        printf("6. Clear the playlist\n");
        printf("7. Remove a song from the playlist\n");
        printf("8. Shuffle the playlist\n");
        printf("9. Resume the paused song\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    char title[MAX_TITLE_LENGTH];
                    char filename[MAX_FILENAME_LENGTH];
                    printf("Enter the title of the song: ");
                    scanf("%s", title);
                    printf("Enter the filename of the song: ");
                    scanf("%s", filename);
                    addSong(&player, title, filename);
                    printf("Song added to the playlist.\n");
                }
                break;
            case 2:
                displayPlaylist(&player);
                break;
            case 3:
                playNextSong(&player);
                break;
            case 4:
                pauseSong(&player);
                break;
            case 5:
                if (player.currentSong != NULL) {
                    printf("Stopping: %s\n", player.currentSong->title);
                    PlaySound(NULL, NULL, 0);
                    player.currentSong = NULL;
                    player.currentIndex = 0;
                } else {
                    printf("No song is currently playing.\n");
                }
                break;
            case 6:
                {
                    SongNode* current = player.playlistHead;
                    while (current != NULL) {
                        SongNode* temp = current;
                        current = current->next;
                        free(temp);
                    }
                    player.playlistHead = player.playlistTail = NULL;
                    player.currentSong = NULL;
                    player.isPaused = FALSE;
                    player.numSongs = 0;
                    player.currentIndex = 0;
                    printf("Playlist cleared.\n");
                }
                break;
            case 7:
                {
                    int index;
                    printf("Enter the index of the song to remove: ");
                    scanf("%d", &index);
                    if (index < 1 || index > player.numSongs) {
                        printf("Invalid index.\n");
                    } else {
                        SongNode* current = player.playlistHead;
                        SongNode* previous = NULL;
                        int i = 1;
                        while (i < index) {
                            previous = current;
                            current = current->next;
                            i++;
                        }
                        if (previous == NULL) {
                            player.playlistHead = current->next;
                        } else {
                            previous->next = current->next;
                        }
                        if (current == player.currentSong) {
                            printf("Stopping: %s\n", player.currentSong->title);
                            PlaySound(NULL, NULL, 0);
                            player.currentSong = NULL;
                            player.currentIndex = 0;
                        }
                        if (current == player.playlistTail) {
                            player.playlistTail = previous;
                        }
                        free(current);
                        player.numSongs--;
                        printf("Song removed from the playlist.\n");
                    }
                }
                break;
            case 8:
                printf("Shuffling the playlist...\n");
                // TODO: Implement shuffle function
                break;
            case 9:
                if (player.isPaused) {
                    printf("Resuming: %s\n", player.currentSong->title);
                    PlaySound(player.currentSong->filename, NULL, SND_FILENAME | SND_ASYNC);
                    player.isPaused = FALSE;
                } else {
                    printf("No paused song to resume.\n");
                }
                break;
            case 10:
                printf("Exiting the music player.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
 
    return 0;
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
    SongNode* current = player->playlistHead;
    while (current != NULL) {
        printf("Include '%s' in the new playlist? (1 for Yes / 0 for No): ", current->title);
        int include;
        scanf("%d", &include);
 
        if (include == 1) {
            fprintf(newPlaylistFile, "%s,%s\n", current->title, current->filename);
        }
        current = current->next;
    }
 
    fclose(newPlaylistFile);
    printf("New playlist '%s' created successfully.\n", newPlaylistFilename);
}

void playNextSong(MusicPlayer* player) {
    if (player->currentSong != NULL) {
        player->currentSong = player->currentSong->next;
        if (player->currentSong != NULL) {
            printf("Playing next song: %s\n", player->currentSong->title);
            playSong(player->currentSong->filename, &(player->isPaused));
            player->currentSongIndex++;
        } else {
            printf("No more songs in the playlist.\n");
            player->currentSongIndex = -1;
        }
    } else {
        printf("No song is currently playing.\n");
    }
}

void playPreviousSong(MusicPlayer *player) {
    if (player->currentSongIndex != -1) {
        SongNode* current = player->playlistHead;
        for (int i = 0; i < player->currentSongIndex - 1; i++) {
            current = current->next;
        }
        player->currentSong = current;
        printf("Playing previous song: %s\n", player->currentSong->title);
        playSong(player->currentSong->filename, &(player->isPaused));
        player->currentSongIndex--;
    } else {
        printf("No song is currently playing.\n");
    }
}

int main() {
    MusicPlayer player;
    player.playlistHead = player.playlistTail = player.currentSong = NULL;
    player.isPaused = FALSE;
    player.numSongs = 0;
    player.currentSongIndex = -1;

    // Read songs from default playlist CSV file
    readSongsFromPlaylistCSV(&player, "playlist.csv");
    while (1) {
        int choice;
        printf("\nMenu:\n1. Display Playlist\n2. Play Song\n3. Create New Playlist\n4. Play Next Song\n5. Play Previous Song\n6. Play Song from New Playlist\n7. Display New Playlist\n8. Pause\n9. Resume\n10. Exit\nEnter your choice: ");
        scanf("%d", &choice);
 
        switch (choice) {
            case 1:
                displayPlaylist(&player);
                break;
            case 2: {
                int songIndex;
                printf("Enter song number to play: ");
                scanf("%d", &songIndex);
                if (songIndex > 0 && songIndex <= player.numSongs) {
                    SongNode* current = player.playlistHead;
                    for (int i = 0; i < songIndex - 1; i++) {
                        current = current->next;
                    }
                    player.currentSong = current;
                    player.currentSongIndex = songIndex - 1;
                    playSong(player.currentSong->filename, &(player.isPaused));
                } else {
                    printf("Invalid song number.\n");
                }
                break;
            }
            case 3:
                createNewPlaylist(&player);
                break;
            case 4:
                playNextSong(&player);
                break;
            case 5:
                playPreviousSong(&player);
                break;
            case 6: {
                char newPlaylistFilename[MAX_FILENAME_LENGTH];
                printf("Enter the name of the new playlist to play from (without extension): ");
                scanf("%s", newPlaylistFilename);
                strcat(newPlaylistFilename, ".csv");
                MusicPlayer newPlayer;
                newPlayer.playlistHead = newPlayer.playlistTail = newPlayer.currentSong = NULL;
                newPlayer.isPaused = FALSE;
                newPlayer.numSongs = 0;
                newPlayer.currentSongIndex = -1;
                readSongsFromPlaylistCSV(&newPlayer, newPlaylistFilename);
 
                displayPlaylist(&newPlayer);
 
                int songIndex;
                printf("Enter song number to play from the new playlist: ");
                scanf("%d", &songIndex);
                if (songIndex > 0 && songIndex <= newPlayer.numSongs) {
                    SongNode* current = newPlayer.playlistHead;
                    for (int i = 0; i < songIndex - 1; i++) {
                        current = current->next;
                    }
                    newPlayer.currentSong = current;
                    newPlayer.currentSongIndex = songIndex - 1;
                    playSong(newPlayer.currentSong->filename, &(newPlayer.isPaused));
                    player = newPlayer;
                } else {
                    printf("Invalid song number.\n");
                }
                break;
            }
            case 7:
                // Display songs from the new playlist
                readSongsFromPlaylistCSV(&player, "new_playlist.csv");
                displayPlaylist(&player);
                break;
            case 8:
                if (player.currentSongIndex != -1) {
                    pauseSong(&(player.isPaused));
                } else {
                    printf("No song is currently playing.\n");
                }
                break;
            case 9:
                if (player.isPaused) {
                    resumeSong(player.currentSong->filename, &(player.isPaused));
                } else {
                    printf("No paused song to resume.\n");
                }
                break;
            case 10:
                printf("Exiting the music player.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
 
    return 0;
}