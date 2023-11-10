#include "game.h"
#include "console.h"
#include "highscore.h"
#include "draw.h"
#include <unistd.h>
#include <windows.h>

int main() {
    // Desired console window size
    int consoleWidth = 700;  // Adjust as necessary
    int consoleHeight = 600; // Adjust as necessary
    bool gameEnded;

    // Resize and center the console window
    ResizeAndCenterConsole(consoleWidth, consoleHeight);

    while(!gameEnded) {

        // Initialize variables, set starting positions, etc.
        LoadHighScores();
        Setup();
        Draw(); // Draw the initial state

        // Game loop
        while (!gameOver) {
        Input();
        Algorithm();
        Draw();
        Sleep(120); // slow down the game for visibility
        }

        gameEnded = GameOver(); // Display game over message and prompt for replay
    }
    return 0;
}