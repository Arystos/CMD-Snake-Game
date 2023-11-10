#include "game.h"
#include "console.h"
#include "highscore.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h> // for Windows API functions
#include <vector>

using namespace std;

Direction dir;
const int width = 20;
const int height = 20;
int x, y, score;
bool gameOver;
vector<pair<int, int>> tail;
vector<pair<int, int>> fruits;
int moveCounter = 0;                 // Counter for moves
const int FRUIT_SPAWN_INTERVAL = 10; // Fruit spawn every 10 moves

const char WALL = '#';
const char SNAKE_HEAD = 'O';
const char FRUIT = 'F';
const char SNAKE_BODY = 'o';
const char EMPTY_SPACE = ' ';

void SpawnFruit()
{
    // Spawn a new fruit at a random location
    int fruitX = rand() % (width - 2) + 1;
    int fruitY = rand() % (height - 2) + 1;
    fruits.push_back(make_pair(fruitX, fruitY));
}

void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    srand(time(0));
    SpawnFruit();
    score = 0;
}

void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
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
            switch (getch())
            {        // now we read the next character which will be the actual arrow key
            case 75: // Left arrow key
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case 77: // Right arrow key
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            case 72: // Up arrow key
                if (dir != DOWN)
                    dir = UP;
                break;
            case 80: // Down arrow key
                if (dir != UP)
                    dir = DOWN;
                break;
            }
        }
    }
}

void Algorithm()
{
    pair<int, int> prev(x, y);
    pair<int, int> prev2;
    int n = tail.size();
    if (n > 0)
    {
        tail[0] = prev;
        for (int i = 1; i < n; i++)
        {
            prev2 = tail[i];
            tail[i] = prev;
            prev = prev2;
        }
    }
    switch (dir)
    {
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

    if (x >= width)
        x = 0;
    else if (x < 0)
        x = width - 1;
    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    for (int i = 0; i < n; i++)
    {
        if (tail[i].first == x && tail[i].second == y)
        {
            gameOver = true;
        }
    }

    // Increase moveCounter and spawn fruit if needed
    moveCounter++;
    if (moveCounter >= FRUIT_SPAWN_INTERVAL) {
        SpawnFruit();
        moveCounter = 0;
    }

    // Check if snake eats a fruit
    for (auto it = fruits.begin(); it != fruits.end();) {
        if (x == it->first && y == it->second) {
            score += 10;
            it = fruits.erase(it); // Remove eaten fruit
            tail.push_back({ x, y });
        } else {
            ++it;
        }
    }
}

void GameOver()
{
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