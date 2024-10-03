#include "../inc/folder_manager.hpp"

using namespace std;

int currentTrack = 0;       // Keep track of the current song
vector<string> folder;      // Store all the songs
bool isPlaying = true;      // Check if music is playing
bool isExiting = false;     // Flag to indicate if the program is exiting

// Function to play music
void playMusic(const string& filename) {
    Mix_Music* music = Mix_LoadMUS(filename.c_str());
    if (!music) {
        cout << "Can't load music: " << Mix_GetError() << endl;
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    if (Mix_PlayMusic(music, 0) == -1) {  // Play the music once
        cout << "Can't play music: " << Mix_GetError() << endl;
    } else {
        isPlaying = true;
    }
}

// Callback function when music finishes
void onMusicFinished() {
    if (isExiting) {
        return;  // Prevent playing the next track if the program is quitting
    }

    currentTrack = (currentTrack + 1) % folder.size();  // Move to the next track
    cout << "\nNext track: " << folder[currentTrack] << endl;
    playMusic(folder[currentTrack]);  // Play the next track
}

void managerFolder() {
    // SDL init
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cout << "SDL can't init: " << SDL_GetError() << endl;
        return;
    }

    // Mixer init
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "Mixer can't init: " << Mix_GetError() << endl;
        SDL_Quit();
        return;
    }

    // Read MP3 files
    string folderPath;
    cout << "Enter the path of the folder you want to play: ";
    cin >> folderPath;

    for (const auto& entry : filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".mp3") {
            folder.push_back(entry.path().string());
        }
    }

    if (folder.empty()) {
        cout << "No MP3 files in your folder" << endl;
        Mix_CloseAudio();
        SDL_Quit();
        return;
    }

    currentTrack = 0;
    char choice;

    // Play the first track
    cout << "\nPlaying: " << folder[currentTrack] << endl;
    playMusic(folder[currentTrack]);

    // Hook the function to play the next song when the current one finishes
    Mix_HookMusicFinished(onMusicFinished);

    while (true) {
        cout << "\nType 'p' to pause the song\n";
        cout << "Type 'r' to resume the song\n";
        cout << "Type 'n' to next the song\n";
        cout << "Type 'b' to back the song\n";
        cout << "Type 'q' to quit the playlist\n";
        cout << "Controls: [p]ause, [r]esume, [n]ext, [b]ack, [q]uit\n";
        cin >> choice;

        switch (choice) {
            case 'p':
                if (isPlaying) {
                    Mix_PauseMusic();
                    isPlaying = false;
                    cout << "Music is paused.\n";
                } else {
                    cout << "Music is already paused.\n";
                }
                break;

            case 'r':
                if (!isPlaying) {
                    Mix_ResumeMusic();
                    isPlaying = true;
                    cout << "Music resumed.\n";
                } else {
                    cout << "Music is already playing.\n";
                }
                break;

            case 'n':
                currentTrack = (currentTrack + 1) % folder.size();  // Move to next track
                cout << "\nNext track: " << folder[currentTrack] << endl;
                playMusic(folder[currentTrack]);
                break;

            case 'b':
                currentTrack = (currentTrack - 1 + folder.size()) % folder.size();  // Move to previous track
                cout << "\nPrevious track: " << folder[currentTrack] << endl;
                playMusic(folder[currentTrack]);
                break;

            case 'q':
                // Print exiting message and clean up
                cout << "Exiting..." << endl;
                break;


            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}
