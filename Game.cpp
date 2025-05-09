#include "candy.h"

int main() {
    int numStars;
    int timeLimit;
    cout << "Welcome to the Candy Grid Game!" << endl;

    // Input validation for number of stars
    while (true) {
        cout << "Enter number of stars (between 5 and 50): ";
        cin >> numStars;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 5 and 50.\n";
            continue;
        }

        if (numStars >= 5 && numStars <= 50) break;
        cout << "Invalid number of stars! Please try again.\n";
    }

    // Input validation for time limit
    while (true) {
        cout << "Enter time in seconds: ";
        cin >> timeLimit;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number of seconds! Please try again.\n";
            continue;
        }

        if (timeLimit <= 0) {
            cout << "Invalid input! Please enter a number greater than 0.\n";
            continue;
        }

        break;
    }

    // Create a grid and start the timer
    Grid grid(numStars, timeLimit);
    thread timerThread(&Grid::updateTimer, &grid);

    // Main game loop
    while (true) {
        grid.display();

        if (grid.movesRemaining <= 0 || grid.timeRemaining <= 0) {
            cout << "Game Over!" << endl;
            break;
        }

        char choice;
        cout << "Do you want to quit the game wrtie q and else press any key? (Q): ";
        cin >> choice;

        if (choice == 'q' || choice == 'Q') {
            cout << "Exiting the game. Goodbye!" << endl;
            break; // Exit the game loop
        }


        int x1, y1, x2, y2;
        cout << "Enter coordinates of candy to swipe (y1 x1 y2 x2): ";
        cin >> x1 >> y1 >> x2 >> y2;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter four numbers.\n";
            continue;
        }

        --x1; --y1; --x2; --y2; // Adjust coordinates for 0-based indexing
        grid.swipe(x1, y1, x2, y2);  // Perform swipe and check matches
    }

    // Wait for the timer thread to finish
    timerThread.join();

    return 0;
}