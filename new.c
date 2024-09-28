#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define MAX_SONGS 100
#define MAX_PATH_LEN 260

void playSong(const char *song)
{
    char command[256];
    sprintf(command, "open \"%s\" type mpegvideo alias mySong", song);
    mciSendString(command, NULL, 0, NULL);
    mciSendString("play mySong", NULL, 0, NULL);
}

void stopSong()
{
    mciSendString("stop mySong", NULL, 0, NULL);
    mciSendString("close mySong", NULL, 0, NULL);
}

void pauseSong()
{
    mciSendString("pause mySong", NULL, 0, NULL);
}

void resumeSong()
{
    mciSendString("play mySong", NULL, 0, NULL);
}

void fastForward(int seconds)
{
    char command[256];
    sprintf(command, "seek mySong to %d", seconds * 1000); // seconds to milliseconds
    mciSendString(command, NULL, 0, NULL);
}

void listSongs(const char *folderPath, char songs[MAX_SONGS][MAX_PATH_LEN], int *songCount)
{
    struct dirent *entry;
    DIR *dir = opendir(folderPath);
    if (dir == NULL)
    {
        printf("Could not open directory: %s\n", folderPath);
        return;
    }

    *songCount = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            if (strstr(entry->d_name, ".wav") || strstr(entry->d_name, ".mp3") || strstr(entry->d_name, ".m4a"))
            {
                snprintf(songs[*songCount], MAX_PATH_LEN, "%s\\%s", folderPath, entry->d_name);
                (*songCount)++;
            }
        }
    }
    closedir(dir);
}

int main()
{
    char folderPath[MAX_PATH_LEN];
    char songs[MAX_SONGS][MAX_PATH_LEN];
    int songCount = 0;

    printf("Songs ");
    scanf("%s", folderPath);

    listSongs(folderPath, songs, &songCount);

    if (songCount == 0)
    {
        printf("No audio files found in the specified folder.\n");
        return 0;
    }

    // Display available songs
    printf("Available songs:\n");
    for (int i = 0; i < songCount; i++)
    {
        printf("%d. %s\n", i + 1, songs[i]);
    }

    int choice;
    while (1)
    {
        printf("\n1. Play Song\n");
        printf("2. Stop Song\n");
        printf("3. Pause Song\n");
        printf("4. Resume Song\n");
        printf("5. Fast Forward (seconds)\n");
        printf("6. Change Song\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int songIndex;
            printf("Select a song to play (1-%d): ", songCount);
            scanf("%d", &songIndex);
            if (songIndex >= 1 && songIndex <= songCount)
            {
                playSong(songs[songIndex - 1]);
            }
            else
            {
                printf("Invalid song selection.\n");
            }
            break;
        }
        case 2:
            stopSong();
            break;
        case 3:
            pauseSong();
            break;
        case 4:
            resumeSong();
            break;
        case 5:
        {
            int seconds;
            printf("Enter seconds to fast forward: ");
            scanf("%d", &seconds);
            fastForward(seconds);
            break;
        }
        case 6:
        {
            int songIndex;
            printf("Select a song to change to (1-%d): ", songCount);
            scanf("%d", &songIndex);
            if (songIndex >= 1 && songIndex <= songCount)
            {
                stopSong(); // Stop current song before changing
                playSong(songs[songIndex - 1]);
            }
            else
            {
                printf("Invalid song selection.\n");
            }
            break;
        }
        case 7:
            stopSong();
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
