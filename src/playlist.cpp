#include "../inc/playlist.hpp"
#include <iostream>

Playlist::Playlist(std::string name) : name(name), currentSongIndex(0) {}

void Playlist::addFile(const File& file) {
    files.push_back(file);
}

void Playlist::removeFile(const std::string& filePath) {
    for (auto it = files.begin(); it != files.end(); ++it) {
        if (it->getFilePath() == filePath) {
            files.erase(it);
            return;
        }
    }
}

void Playlist::viewPlaylist() const {
    std::cout << "Playlist: " << name << std::endl;
    for (const auto& file : files) {
        std::cout << file.getFilePath() << std::endl;
    }
}

void Playlist::playCurrentSong() {
    if (!files.empty()) {
        files[currentSongIndex].play();
    } else {
        std::cerr << "No files in the playlist to play.\n";
    }
}

void Playlist::pauseCurrentSong() {
    files[currentSongIndex].pause();
}

void Playlist::resumeCurrentSong() {
    files[currentSongIndex].resume();
}

void Playlist::nextSong() {
    if (currentSongIndex < files.size() - 1) {
        currentSongIndex++;
        playCurrentSong();
    } else {
        std::cout << "Reached the end of the playlist.\n";
    }
}

void Playlist::previousSong() {
    if (currentSongIndex > 0) {
        currentSongIndex--;
        playCurrentSong();
    } else {
        std::cout << "Already at the first song.\n";
    }
}

void Playlist::playPlaylistWithControls() {
    char command;
    playCurrentSong();
    while (true) {
        std::cout << "\nType 'p' to pause the song\n";
        std::cout << "Type 'r' to resume the song\n";
        std::cout << "Type 'n' to next the song\n";
        std::cout << "Type 'b' to back the song\n";
        std::cout << "Type 'q' to quit the playlist\n";
        std::cout << "Controls: [p]ause, [r]esume, [n]ext, [b]ack, [q]uit\n";
        std::cin >> command;

        switch (command) {
            case 'p':
                pauseCurrentSong();
                break;
            case 'r':
                resumeCurrentSong();
                break;
            case 'n':
                nextSong();
                break;
            case 'b':
                previousSong();
                break;
            case 'q':
                std::cout << "Stopping playback.\n";
                return;
            default:
                std::cerr << "Invalid command.\n";
        }
    }
}

std::string Playlist::getName() const {
    return name;
}
