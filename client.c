#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void playSong(char *song) {
    PlaySound(song, NULL, SND_FILENAME | SND_ASYNC);
}

void stopSong() {
    PlaySound(NULL, NULL, 0);  // Stop current sound
}

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Setup server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to server IP if not local
    serverAddr.sin_port = htons(PORT);

    // Connect to the server
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    // Main loop to listen for commands from the server
    while (1) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            break; // Connection closed
        }
        
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        
        if (strcmp(buffer, "PLAY") == 0) {
            // Play the last song received (you need to manage the song name properly)
            playSong("songs/song1.wav"); // Replace with the appropriate song based on your logic
        } else if (strcmp(buffer, "STOP") == 0) {
            stopSong();
        } else if (strcmp(buffer, "RESUME") == 0) {
            playSong("songs/song1.wav"); // Resume the same song
        } else if (strcmp(buffer, "CHANGE") == 0) {
            // You may want to implement logic to change the song
            playSong("songs/song2.wav"); // Change to a different song
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
