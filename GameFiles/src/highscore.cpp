#include "highscore.h"
#include "console.h"
#include <fstream>
#include <iostream>

using namespace std;

map<int, string, greater<int>> highscores;

void LoadHighScores() {
    // Implement LoadHighScores here
    std::ifstream file("scores.txt");
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