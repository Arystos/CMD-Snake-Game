#ifndef GAME_H
#define GAME_H

#include <vector>
#include <utility>

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

extern Direction dir;
extern const int width;
extern const int height;
extern int x, y, fruitX, fruitY, score;
extern bool gameOver;
extern std::vector<std::pair<int, int>> tail;

void Setup();
void Draw();
void Input();
void Algorithm();
void GameOver();

#endif