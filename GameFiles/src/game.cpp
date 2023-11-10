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
vector<pair<int, int>> obstacles;
int moveCounter = 0;                 // Counter for moves
int moveCounterForObstacles = 0;     // Counter for obstacles
int moveCounterForRemovingObstacles = 0; // Counter for removing obstacles
const int FRUIT_SPAWN_INTERVAL = 16; // Fruit spawn every 10 moves

void SpawnFruit()
{
    // Spawn a new fruit at a random location
    int fruitX = rand() % (width - 2) + 1;
    int fruitY = rand() % (height - 2) + 1;
    fruits.push_back(make_pair(fruitX, fruitY));
}

void SpawnObstacles() {
    // Spawn one obstacle at a random location, avoid spawning on the snake or on the tails
    int obstacleX = rand() % (width - 2) + 1;
    int obstacleY = rand() % (height - 2) + 1;
    obstacles.push_back(make_pair(obstacleX, obstacleY));
    for (int i = 0; i < tail.size(); i++) { // Avoid spawning on the snake's tail
        if (obstacleX == tail[i].first && obstacleY == tail[i].second) {
            obstacles.pop_back();
            SpawnObstacles();
        }
    }
    if (obstacleX == x && obstacleY == y) { // Avoid spawning on the snake's head
        obstacles.pop_back();
        SpawnObstacles();
    }
}

void RemoveObstacles() {
    // Remove obstacles in order of creation
    obstacles.erase(obstacles.begin());
}

void Setup() // Initialize variables, set starting positions, etc.
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    srand(time(0));
    SpawnFruit();
    SpawnObstacles();
    RemoveObstacles();
    score = 0;
}

void Input() // Get input from the user
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

void Algorithm() // Handle game logic
{
    pair<int, int> prev(x, y);  // Previous position of the snake's head
    pair<int, int> prev2;       // Previous position of the snake's body
    int n = tail.size();

    if (n > 0) // Move the tail
    {
        tail[0] = prev;
        for (int i = 1; i < n; i++)
        {
            prev2 = tail[i];
            tail[i] = prev;
            prev = prev2;
        }
    }

    switch (dir) // Move the snake in the direction specified by the user
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

    // Check if snake hits a wall
    if (x >= width)
        x = 0;
    else if (x < 0)
        x = width - 1;
    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    // Check if snake hits itself or an obstacle
    for (int i = 0; i < n; i++)
    {
        if (tail[i].first == x && tail[i].second == y)
        {
            gameOver = true;
        }

        if (x == obstacles[i].first && y == obstacles[i].second) {
            gameOver = true;
        }
    }

    // Increase moveCounter and spawn fruit if needed
    moveCounter++;
    if (moveCounter >= FRUIT_SPAWN_INTERVAL) { // Spawn fruit every 10 moves
        SpawnFruit();
        moveCounter = 0;
    }

    // Spawn obstacles
    moveCounterForObstacles++;
    if (moveCounterForObstacles >= FRUIT_SPAWN_INTERVAL * 2) {
        SpawnObstacles();
        moveCounterForObstacles = 0;
    }
    if (moveCounterForObstacles >= FRUIT_SPAWN_INTERVAL * 4) {
        RemoveObstacles();
        moveCounterForRemovingObstacles = 0;
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

// Restart snake, fruits, obstacles, score, movecounters, tail, etc.
void RestartVariables() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    srand(time(0));
    fruits.clear();
    obstacles.clear();
    tail.clear();
    SpawnFruit();
    SpawnObstacles();
    RemoveObstacles();
    score = 0;
    moveCounter = 0;
    moveCounterForObstacles = 0;
    moveCounterForRemovingObstacles = 0;
}

bool GameOver()
{
    system("cls"); // Clear the screen
    cout << "Game Over!\nScore: " << score << endl;
    cout << "Enter your name: ";
    string name;
    cin >> name;
    StoreHighScore(name, score);
    char choice;

    // Reload scores in case the new score is a high score
    highscores.clear();
    LoadHighScores();

    // Display high scores without extra spacing
    DisplayHighScores(false);

    // Ask to the player if he want to continue again (y for yes and n for no)
    cout << "\nDo you want to play again? (y/n): ";
    cin >> choice;

    // Handle invalid input
    while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
        system("cls");
        cout << "\nFucking dumb! Enter a valid input!";
        cin >> choice;
    }

    if (choice == 'y' || choice == 'Y') {
        system("cls");
        RestartVariables();
        return false; // Restart the game
    }
        
    else
        return true;
}