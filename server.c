#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Global variable to keep track of the current song
char *currentSong = NULL;
HWND hwnd = NULL;

// Function to play a song
void playSong(char *song) {
    if (currentSong != NULL) {
        PlaySound(NULL, NULL, 0);  // Stop current sound
    }
    currentSong = song;
    PlaySound(currentSong, NULL, SND_FILENAME | SND_ASYNC);
}

// Function to stop the current song
void stopSong() {
    if (currentSong != NULL) {
        PlaySound(NULL, NULL, 0);  // Stop current sound
        currentSong = NULL;
    }
}

// Function to pause the current song
void pauseSong() {
    // There is no direct way to pause a sound using PlaySound API,
    // Instead, you can stop the sound and play it again later.
}

// Function to resume the current song
void resumeSong() {
    if (currentSong != NULL) {
        PlaySound(currentSong, NULL, SND_FILENAME | SND_ASYNC);
    }
}

// Function to change the song
void changeSong(char *song) {
    stopSong();
    playSong(song);
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    
    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Setup server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    
    // Bind the socket
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    // Listen for incoming connections
    listen(serverSocket, MAX_CLIENTS);
    
    printf("Server listening on port %d...\n", PORT);
    
    // Main server loop
    while (1) {
        printf("Waiting for a client to connect...\n");
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        printf("Client connected!\n");
        
        // Handle client commands
        while (1) {
            int choice;
            printf("\n1. Play Song\n2. Stop Song\n3. Pause Song\n4. Resume Song\n5. Fast Forward (seconds)\n6. Change Song\n7. Exit\nEnter your choice: ");
            scanf("%d", &choice);
            if (choice == 7) {
                break;
            } else if (choice == 1) {
                // Play song logic
                // Display available songs
                printf("Select a song to play:\n1. song1.wav\n2. song2.wav\n");
                int songChoice;
                scanf("%d", &songChoice);
                if (songChoice == 1) {
                    playSong("songs/song1.wav");
                } else if (songChoice == 2) {
                    playSong("songs/song2.wav");
                }
            } else if (choice == 2) {
                stopSong();
            } else if (choice == 3) {
                pauseSong();
            } else if (choice == 4) {
                resumeSong();
            } else if (choice == 5) {
                int seconds;
                printf("Enter seconds to fast forward: ");
                scanf("%d", &seconds);
                // Fast forward logic can be complex; this is a placeholder
            } else if (choice == 6) {
                // Change song logic
                printf("Select a song to change to:\n1. song1.wav\n2. song2.wav\n");
                int changeSongChoice;
                scanf("%d", &changeSongChoice);
                if (changeSongChoice == 1) {
                    changeSong("songs/song1.wav");
                } else if (changeSongChoice == 2) {
                    changeSong("songs/song2.wav");
                }
            }
        }
        
        closesocket(clientSocket);
        printf("Client disconnected.\n");
    }
    
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
