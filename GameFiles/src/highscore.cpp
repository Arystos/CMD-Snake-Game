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
    ofstream file("/scores.txt", ios::out | ios::trunc);
    if (file.is_open()) {
        for (const auto& pair : highscores) {
            file << pair.first << " " << pair.second << endl;
        }
        file.close();
    }
}

void DisplayHighScores(bool includeSpacing) {
    int posY;
    int height = 20;
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