#include <iostream>
#include <ncurses.h>
#include <string>
#include <filesystem>
#include <cstring>
#include "../inc/menu_file.hpp"
#include "../inc/menu_showfile.hpp"
#include "../inc/menu_playlist.hpp"
#include "../inc/menu_play.hpp"

#define HEIGHT 10
#define WIDTH  55

using namespace std;

void drawVolumeBar(WINDOW* win, int currentVolume, int maxVolume) {
    int barWidth = 50;  // Chiều dài thanh tiến trình
    int filledWidth = (currentVolume * barWidth) / maxVolume;  // Độ dài phần được lấp đầy dựa trên âm lượng hiện tại

    mvwprintw(win, 2, 2, "Use LEFT/RIGHT to adjust volume, ESC to quit.");
    // Vẽ thanh trống
    mvwprintw(win, 4, 2, "[");
    for (int i = 0; i < barWidth; ++i) {
        if (i < filledWidth) {
            wattron(win, A_REVERSE);   // Đánh dấu phần lấp đầy
            waddch(win, ' ');          // Lấp đầy phần tương ứng với âm lượng hiện tại
            wattroff(win, A_REVERSE);  // Tắt phần đánh dấu
        } else {
            waddch(win, ' ');          // Phần còn lại là khoảng trống
        }
    }
    wprintw(win, "]");

    // Hiển thị mức âm lượng
    mvwprintw(win, 6, 2, "Volume: %d%%", (currentVolume * 100) / maxVolume);
}

void manageVolume(WINDOW* main_win) {
    // Keep ncurses running, no need to end it
    int maxVolume = MIX_MAX_VOLUME; // Max volume value (100%)
    int currentVolume = Mix_VolumeMusic(-1); // Get current music volume
    if (currentVolume < 0) currentVolume = 0; // Check if it failed to get volume

    // Immediately show volume bar when entering volume menu
    clear();
    drawVolumeBar(main_win, currentVolume, maxVolume);
    refresh();
    wrefresh(main_win);

    bool exitVolumeMenu = false;

    while (!exitVolumeMenu) {
        int key = getch();
        switch (key) {
            case KEY_LEFT:
                if (currentVolume > 0) {
                    currentVolume -= 5;  // Decrease volume
                    if (currentVolume < 0) currentVolume = 0;  // Ensure it doesn't go below 0
                }
                break;
            case KEY_RIGHT:
                if (currentVolume < maxVolume) {
                    currentVolume += 5;  // Increase volume
                    if (currentVolume > maxVolume) currentVolume = maxVolume;  // Ensure it doesn't exceed max
                }
                break;
            case 27:  // ESC key to exit
                exitVolumeMenu = true;
                break;
        }

        // Convert current volume to SDL volume and apply it
        Mix_VolumeMusic(currentVolume);  // Update SDL volume for music

        // Redraw the volume bar after every change
        werase(main_win);
        box (main_win, 0, 0);  // Draw box around the volume bar
        drawVolumeBar (main_win, currentVolume, maxVolume);
        wrefresh (main_win);
    }
}

void displayMenu(WINDOW* main_win, int highlight) {
    werase(main_win);                                         // Delete window
    box(main_win,0,0);                                        // Bounding window
    refresh();                                                
    mvwprintw(main_win,1,15,"MEDIA PLAYER VERSION 1.0");       // Print at y = 1, x = 4 in window
    mvwprintw(main_win,2,1,"=====================================================");

    const char *choices[] = {"Play song", "Metadata", "Playlists", "View media file", "Adjust Volume", "Exit"};
    int num_choices = sizeof(choices) / sizeof(choices[0]);   // Number of menu funcs

    for(int i = 0; i < num_choices; i++) {
        int x_position = (WIDTH - strlen(choices[i])) / 2;
        if(i == highlight) {
            wattron(main_win,A_REVERSE);                      // Change color when use arrow key
            mvwprintw(main_win, 3 + i, x_position, "%s", choices[i]);
            wattroff(main_win,A_REVERSE);                     // Off color
        }
        else {
            mvwprintw(main_win, 3 + i, x_position, "%s", choices[i]);  // Print menu funcs no color 
        }
    }

    wrefresh(main_win);                                       // Update and refresh window
}

void manageMetadata() {
    // Exit ncurses mode to allow normal terminal I/O
    endwin();

    // Call the function from menu_file.cpp to manage metadata
    displayMetadataMenu();

    // After managing metadata, reinitialize ncurses and return to main menu
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void manageMediafiles(){
    // Exit ncurses mode to allow normal terminal I/O
    endwin();

    // Call the function from menu_showfile.cpp to manage metadata
    displayMediaFile();

    // After managing metadata, reinitialize ncurses and return to main menu
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void managePlaylist(){
    // Exit ncurses mode to allow normal terminal I/O
    endwin();

    // Call the function from menu_playlist.cpp to manage playlists
    displayPLaylistMenu();

    // After managing playlist, reinitialize ncurses and return to main menu
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void managePlaySong(){
    // Exit ncurses mode to allow normal terminal I/O
    endwin();

    // Call the function from menu_playlist.cpp to manage playlists
    displaySongMenu();

    // After managing playlist, reinitialize ncurses and return to main menu
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

int main(){
    int highlight = 0;

    // Init ncurses
    initscr();                                                // Init 
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);                                     // Enable keyboard
    curs_set(0);

    // Display main menu
    int height, width;
    getmaxyx(stdscr, height, width);
    WINDOW* main_win = newwin(HEIGHT, WIDTH, (height - HEIGHT) / 2, (width - WIDTH) / 2);
    displayMenu(main_win, highlight);
    getch();

    int choice;
    bool exitProgram = false;

    while (!exitProgram) {
        choice = getch();

        switch (choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? 5 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == 5) ? 0 : highlight + 1;
                break;
            case 10: // Enter key
                switch (highlight) {
                    case 0:
                        // Play song (implement this later)
                        printf("\e[1;1H\e[2J"); // Clear screen
                        managePlaySong();
                        break;
                    case 1:
                        // Manage metadata
                        printf("\e[1;1H\e[2J"); // Clear screen
                        manageMetadata();
                        break;
                    case 2:
                        // Manage playlist
                        printf("\e[1;1H\e[2J"); // Clear screen
                        managePlaylist();
                        break;
                    case 3:
                        // View media file
                        printf("\e[1;1H\e[2J"); // Clear screen
                        manageMediafiles();
                        break;
                    case 4:
                        printf("\e[1;1H\e[2J"); // Clear screen
                        manageVolume(main_win);
                        break;
                    case 5:
                        exitProgram = true;
                        break;
                }
                break;
        }

        displayMenu(main_win, highlight);
    }

    // Close the audio system
    Mix_CloseAudio();
    SDL_Quit();

    // End ncurses
    endwin();
    return 0;
}
