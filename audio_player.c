#include <stdio.h>
#include <windows.h>

void play_audio(const char *filename) {
    // Play the audio file
    if (PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC) == 0) {
        printf("Error playing sound: %s\n", filename);
    } else {
        printf("Playing: %s\n", filename);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <audio_file.wav>\n", argv[0]);
        return 1;
    }

    play_audio(argv[1]);
    
    // Keep the program running while audio is playing
    printf("Press any key to stop playback...\n");
    getchar(); 
    return 0;
}
