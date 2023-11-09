#include "game.h"
#include "console.h"
#include "highscore.h"
#include <unistd.h>
#include <windows.h>

int main() {
    // Desired console window size
    int consoleWidth = 700;  // Adjust as necessary
    int consoleHeight = 600; // Adjust as necessary

    // Resize and center the console window
    ResizeAndCenterConsole(consoleWidth, consoleHeight);

    LoadHighScores();
    Setup();
    Draw(); // Draw the initial state

    while (!gameOver) {
        Input();
        Algorithm();
        Draw();
        Sleep(100); // slow down the game for visibility
    }

    GameOver();
    return 0;
}