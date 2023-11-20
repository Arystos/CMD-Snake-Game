#include "draw.h"
#include "console.h"
#include "highscore.h"
#include "game.h"
#include <iostream>

using namespace std;

// Constants
extern const char WALL = '#';
extern const char SNAKE_HEAD = 'O';
extern const char FRUIT = 4;
extern const char SNAKE_BODY = 'o';
extern const char EMPTY_SPACE = ' ';
extern const char OBSTACLE = 219;

// ----------------- WALL DRAWING FUNCTIONS -----------------

void DrawTopOrBottomWall() {
    for (int i = 0; i < WIDTH + 2; i++) {
        cout << WALL;
    }
    cout << endl;
}

// ----------------- GAME BOARD DRAWING FUNCTIONS -----------------

bool DrawFruit(int i, int j) {
    for (const auto& fruit : fruits) {
        if (fruit.first == j && fruit.second == i) {
            cout << FRUIT;
            return true;
        }
    }
    return false;
}

bool DrawObstacle(int i, int j) {
    for (const auto& obstacle : obstacles) { // Draw obstacles
        if (obstacle.first == j && obstacle.second == i) { // If the current cell is an obstacle
            cout << OBSTACLE;
            return true;
        }
    }
    return false;
}

bool DrawSnakeBody(int i, int j) {
    for (const auto& segment : tail) {
        if (segment.first == j && segment.second == i) {
            cout << SNAKE_BODY;
            return true;
        }
    }
    return false;
}

void DrawGameBoardCell(int i, int j) {

    if (j == 0) cout << WALL; // Draw left wall

    if (i == y && j == x) {
        cout << SNAKE_HEAD; // Draw the snake's head
    } else if (DrawSnakeBody(i, j)) {
        // Snake body is drawn within the function
    } else if (DrawFruit(i, j)) {
        // Fruit is drawn within the function
    } else if (DrawObstacle(i, j)) {
        // Obstacle is drawn within the function
    }
    else {
        cout << EMPTY_SPACE; // Draw empty space
    }

    if (j == WIDTH - 1) cout << WALL; // Draw right wall
}

void DrawGameBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            DrawGameBoardCell(i, j);
        }
        cout << endl;
    }
}

// ----------------- SCORE DISPLAY FUNCTIONS -----------------

void DisplayHighScores(bool includeSpacing) {
    int posY;
    int height = 20;
    if (includeSpacing) {
        posY = height + 3;
    } else {
        posY = 2;
    }
    int scoreBoardWidth = 25; // Adjust the width as needed
    gotoxy(0, posY++);
    cout << "+";                                            // top-left corner
    for (int i = 0; i < scoreBoardWidth; ++i) cout << "-";  // top border
    cout << "+";                                            // top-right corner

    int count = 0;
    cout << endl;
    for (const auto& pair : highscores) {
        if (count < 3) { // Only display top 3 scores
            gotoxy(0, posY);
            cout << "| " << count + 1 << ". " << pair.second;
            gotoxy(scoreBoardWidth - 7, posY++);
            cout << " - " << pair.first << " |";
        }
        ++count;
    }
    while (count < 3) { // Fill in empty rows if there are less than 3 high scores
        gotoxy(0, posY);
        cout << "|";
        for (int i = 1; i < scoreBoardWidth; ++i) cout << " ";
        cout << "|";
        posY++;
        count++;
    }

    gotoxy(0, posY);
    cout << "+"; // bottom-left corner
    for (int i = 0; i < scoreBoardWidth; ++i) cout << "-"; // bottom border
    cout << "+"; // bottom-right corner
    cout << endl;

    // If includeSpacing is false, remove extra spaces in empty rows
    if (!includeSpacing) {
        while (count < 3) {
            gotoxy(0, posY);
            cout << "|";
            for (int i = 1; i < scoreBoardWidth; ++i) cout << "-";
            cout << "|";
            posY++;
            count++;
        }
    }
}

void DisplayScoreAndHighScores() {
    gotoxy(0, HEIGHT + 2);
    cout << "Score:" << score << endl;
    DisplayHighScores();
}

// ----------------- MAIN DRAW FUNCTION -----------------

void Draw() {
    gotoxy(0, 0);
    DrawTopOrBottomWall();
    DrawGameBoard();
    DrawTopOrBottomWall();
    DisplayScoreAndHighScores();
}