#include "game.h"
#include "console.h"
#include "highscore.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h> // for Windows API functions

using namespace std;

Direction dir;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
bool gameOver;
vector<pair<int, int>> tail;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    srand(time(0));
    fruitX = rand() % (width - 2) + 1; // spawn fruit inside the play area
    fruitY = rand() % (height - 2) + 1; // spawn fruit inside the play area
    score = 0;
}

void Draw() {
    gotoxy(0, 0); // Reset cursor position to top-left corner

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Left wall
            if (j == 0)
                cout << "#"; 

            // Snake head
            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "F"; // Fruit
            else {
                bool print = false;
                // Snake body
                for (int k = 0; k < tail.size(); k++) {
                    if (tail[k].first == j && tail[k].second == i) {
                        cout << "o";
                        print = true;
                        break;
                    }
                }
                // Empty space
                if (!print)
                    cout << " ";
            }

            // Right wall
            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
        
    // Show current score
    gotoxy(0, height + 2);
    cout << "Score:" << score << endl;

    // Show top three high scores
    DisplayHighScores();
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
        case 75: // Left arrow key
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
        case 77: // Right arrow key
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
        case 72: // Up arrow key
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
        case 80: // Down arrow key
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        case 224: // this is the special key prefix for arrow keys
                switch (getch()) { // now we read the next character which will be the actual arrow key
                    case 75: // Left arrow key
                        if (dir != RIGHT) dir = LEFT;
                        break;
                    case 77: // Right arrow key
                        if (dir != LEFT) dir = RIGHT;
                        break;
                    case 72: // Up arrow key
                        if (dir != DOWN) dir = UP;
                        break;
                    case 80: // Down arrow key
                        if (dir != UP) dir = DOWN;
                        break;
                }
        }
    }
}


void Algorithm() {
    pair<int, int> prev(x, y);
    pair<int, int> prev2;
    int n = tail.size();
    if (n > 0) {
        tail[0] = prev;
        for (int i = 1; i < n; i++) {
            prev2 = tail[i];
            tail[i] = prev;
            prev = prev2;
        }
    }
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    for (int i = 0; i < n; i++) {
        if (tail[i].first == x && tail[i].second == y) {
            gameOver = true;
        }
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % (width - 2) + 1;
        fruitY = rand() % (height - 2) + 1;
        tail.push_back({ fruitX, fruitY });
    }
}

void GameOver() {
    system("cls"); // Clear the screen
    cout << "Game Over!\nScore: " << score << endl;
    cout << "Enter your name: ";
    string name;
    cin >> name;
    StoreHighScore(name, score);

    // Reload scores in case the new score is a high score
    highscores.clear();
    LoadHighScores();

    // Display high scores without extra spacing
    DisplayHighScores(false);

    // Wait for input before clearing the screen and ending the game
    cout << "\nPress any key to continue...";
    system("pause>nul"); // no extra characters are shown when a key is pressed

    system("cls"); // Clear the screen one more time before exiting
}