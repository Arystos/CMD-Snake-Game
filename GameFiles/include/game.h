#ifndef GAME_H
#define GAME_H

#include <vector>
#include <utility>

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

extern Direction direction;
extern const int WIDTH;
extern const int HEIGHT;
extern int x, y, fruitX, fruitY, score;
extern bool gameOver;
extern std::vector<std::pair<int, int>> tail;
extern int moveCounterForRemovingObstacles;
extern int moveCounterForObstacles;

void SpawnFruit();
void SpawnObstacles();
void Setup();
void Input();
void Algorithm();
void RestartVariables();
bool GameOver();

#endif