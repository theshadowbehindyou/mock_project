#include <iostream>
#include "../inc/file.hpp"

using namespace std;

void displayMetadataMenu() {
    string filePath;

    // Prompt user for the file path
    cout << "Enter the path to the audio file: ";
    getline(cin, filePath);

    // Check if the file exists
    if (!filesystem::exists(filePath)) {
        cout << "Error: The specified file does not exist. Please try again.\n";
        cout << "\nPress any key to return to the main menu...";
        cin.ignore();  // To clear the input buffer.
        return; // Exit the function if the file doesn't exist
    }

    // Create an instance of File
    File File(filePath);

    int choice;

    do {
        cout << "\nMetadata Management Menu:\n";
        cout << "1. Show Metadata\n";
        cout << "2. Edit Metadata\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(); // Ignore the newline character left in the buffer

        switch (choice) {
            case 1:
                File.show(); // Show metadata
                break;
            case 2:
                File.edit(); // Edit metadata
                break;
            case 3:
                cout << "Exiting metadata management...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 3);
}

void displayPlayFile(){
    string filePath;

    // Prompt user for the file path
    cout << "Enter the path to the audio file: ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, filePath);
    

    // Check if the file exists
    if (!filesystem::exists(filePath)) {
        cout << "Error: The specified file does not exist. Please try again.\n";
        cout << "\nPress any key to return to the main menu...";
        cin.ignore();  // To clear the input buffer.
        return; // Exit the function if the file doesn't exist
    }

    // Initialize SDL2 and SDL_mixer
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! Mix Error: " << Mix_GetError() << endl;
        return;
    }

    // Create File object and play the music
    File file(filePath);
    file.play();

    // Simple loop to handle input
    char command;
    do {
        cout << "\nType 'p' to pause the song\n";
        cout << "Type 'r' to resume the song\n";
        cout << "Type 'q' to quit the song\n";
        cout << "Controls: [p]ause, [r]esume, [q]uit\n";
        cin >> command;

        switch (command) {
            case 'p':
                file.pause();
                break;
            case 'r':
                file.resume();
                break;
            case 'q':
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid command.\n";
                break;
        }
    } while (command != 'q');

}