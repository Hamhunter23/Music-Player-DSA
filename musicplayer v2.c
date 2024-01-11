#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
 
#define MAX_SONGS 500
#define MAX_TITLE_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define MAX_LINE_LENGTH 256
#define MAX_PLAYLIST_NAME_LENGTH 50
 
// Structure to hold information about a song
typedef struct {
    char title[MAX_TITLE_LENGTH];
    char filename[MAX_FILENAME_LENGTH];
} Song;
 
// Structure to manage the music player
typedef struct {
    Song playlist[MAX_SONGS];
    int numSongs;
    int currentSongIndex; // Index of the current playing song (-1 if none)
    BOOL isPaused;        // Flag to track pause state
} MusicPlayer;
 
// Function to add a song to the playlist
void addSong(MusicPlayer *player, const char *title, const char *filename) {
    if (player->numSongs < MAX_SONGS) {
        strcpy(player->playlist[player->numSongs].title, title);
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
        printf("%d. %s\n", i + 1, player->playlist[i].title);
    }
}
 
// Function to play a song
void playSong(const char *filename, BOOL *isPaused) {
    printf("Playing: %s\n", filename);
    if (*isPaused) {
        PlaySound(NULL, 0, 0); // Stop the currently playing sound (if paused)
        *isPaused = FALSE;
    }
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}
 
// Function to pause the current song
void pauseSong(BOOL *isPaused) {
    printf("Pausing...\n");
    *isPaused = TRUE;
    PlaySound(NULL, 0, 0); // Stop the currently playing sound
}
 
// Function to resume playback
void resumeSong(const char *filename, BOOL *isPaused) {
    printf("Resuming...\n");
    *isPaused = FALSE;
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}
 
// Rest of your code remains the same ...
// Function to read songs from a given playlist CSV file and populate the playlist
void readSongsFromPlaylistCSV(MusicPlayer *player, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }
 
    char line[MAX_LINE_LENGTH];
    // Skip the header line
    fgets(line, MAX_LINE_LENGTH, file);
 
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && player->numSongs < MAX_SONGS) {
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
 
// Function to create a new playlist from selected songs
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
 
// Function to play the next song in the playlist
void playNextSong(MusicPlayer *player) {
    if (player->currentSongIndex != -1) {
        player->currentSongIndex = (player->currentSongIndex + 1) % player->numSongs;
        printf("Playing next song: %s\n", player->playlist[player->currentSongIndex].title);
        playSong(player->playlist[player->currentSongIndex].filename, &(player->isPaused));
    } else {
        printf("No song is currently playing.\n");
    }
}
 
// Function to play the previous song in the playlist
void playPreviousSong(MusicPlayer *player) {
    if (player->currentSongIndex != -1) {
        player->currentSongIndex = (player->currentSongIndex - 1 + player->numSongs) % player->numSongs;
        printf("Playing previous song: %s\n", player->playlist[player->currentSongIndex].title);
        playSong(player->playlist[player->currentSongIndex].filename, &(player->isPaused));
    } else {
        printf("No song is currently playing.\n");
    }
}
 
// Rest of your code remains the same ...
 
 
 
int main() {
    MusicPlayer player;
    player.numSongs = 0;
    player.currentSongIndex = -1; // No song playing initially
    player.isPaused = FALSE;
 
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
                    player.currentSongIndex = songIndex - 1;
                    playSong(player.playlist[player.currentSongIndex].filename, &(player.isPaused));
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
                player.numSongs = 0;
                readSongsFromPlaylistCSV(&player, newPlaylistFilename);
 
                displayPlaylist(&player);
 
                int songIndex;
                printf("Enter song number to play from the new playlist: ");
                scanf("%d", &songIndex);
                if (songIndex > 0 && songIndex <= player.numSongs) {
                    player.currentSongIndex = songIndex - 1;
                    playSong(player.playlist[player.currentSongIndex].filename, &(player.isPaused));
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
                    resumeSong(player.playlist[player.currentSongIndex].filename, &(player.isPaused));
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
