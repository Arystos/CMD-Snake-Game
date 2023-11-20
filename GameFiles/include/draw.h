#ifndef DRAW_H
#define DRAW_H

#include <vector>
#include <utility> // for std::pair

// Forward declarations
extern const int WIDTH;
extern const int HEIGHT;
extern int x, y, score;
extern std::vector<std::pair<int, int>> tail;
extern std::vector<std::pair<int, int>> fruits;
extern std::vector<std::pair<int, int>> obstacles;

// Function declarations
void DrawTopOrBottomWall();
bool DrawFruit(int i, int j);
bool DrawObstacle(int i, int j);
bool DrawSnakeBody(int i, int j);
void DrawGameBoardCell(int i, int j);
void DrawGameBoard();
void DisaplayHighScores(bool includeSpacing);
void DisplayScoreAndHighScores();
void Draw();

#endif // DRAW_H