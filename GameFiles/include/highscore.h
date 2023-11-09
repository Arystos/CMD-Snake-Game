#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <map>

using namespace std;

extern map<int, string, greater<int>> highscores;

void LoadHighScores();
void DisplayHighScores(bool includeSpacing = true);
void StoreHighScore(const string& name, int score);

#endif