#include <iostream>
#include <conio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> // for Windows API functions
#include <fstream>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
bool gameOver;
vector<pair<int, int>> tail;
map<int, string, greater<int>> highscores; // Using a map sorted by key (score)

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void LoadHighScores() {
    ifstream file("scores.txt");
    if (file.is_open()) {
        int score;
        string name;
        while (file >> score >> name) {
            highscores[score] = name;
        }
        file.close();
    }
}

void StoreHighScore(const string& name, int score) {
    highscores[score] = name;
    ofstream file("scores.txt", ios::out | ios::trunc);
    if (file.is_open()) {
        for (const auto& pair : highscores) {
            file << pair.first << " " << pair.second << endl;
        }
        file.close();
    }
}

void DisplayHighScores(bool includeSpacing = true) {
    int posY;
    if (includeSpacing) {
        posY = height + 3; // Position to start displaying high scores during the game
    } else {
        posY = 2; // Position to start displaying high scores after the game
    }
    int scoreBoardWidth = 25; // Adjust the width as needed
    gotoxy(0, posY++);
    cout << "+"; // top-left corner
    for (int i = 0; i < scoreBoardWidth; ++i) cout << "-"; // top border
    cout << "+"; // top-right corner

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
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
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

void ResizeAndCenterConsole(int width, int height) {
    // Get the console window handle
    HWND consoleWindow = GetConsoleWindow();

    // Get the current screen width and height
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Calculate the window size and position for centering
    int windowPosX = (screenWidth - width) / 2;
    int windowPosY = (screenHeight - height) / 2;

    // Set the window position and size
    SetWindowPos(consoleWindow, 0, windowPosX, windowPosY, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

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
