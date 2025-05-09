#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <iomanip>
#include <set>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;

class Node {
public:
    int x, y;
    bool hasStar;
    char candyColor;
    Node* left;
    Node* right;
    Node* up;
    Node* down;

    Node(int x, int y, bool hasStar, char candyColor)
        : x(x), y(y), hasStar(hasStar), candyColor(candyColor),
        left(nullptr), right(nullptr), up(nullptr), down(nullptr) {
    }
};

class Grid {
public:
    int movesRemaining;
    int starsCollected;
    int totalStars;
    int timeRemaining;
    int score = 0;
    Node* head;
    const char candyColors[3] = { 'R', 'G', 'B' };

    void initializeGrid();
    void placeStars(int gameLevel);
    Node* getNode(int x, int y);
    char getValidCandyColor(int x, int y);
    bool isValidSwipe(int x1, int y1, int x2, int y2);
    bool checkMatch(int x, int y);
    void removeMatches();
    void refillGrid();
    void checkGameOver();


    Grid(int gameLevel, int timeLimit);
    ~Grid();
    void updateTimer();
    void display();
    void swipe(int x1, int y1, int x2, int y2);
};

#endif // GRID_H