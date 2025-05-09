#include "candy.h"


Grid::Grid(int gameLevel, int timeLimit) : movesRemaining(20), starsCollected(0), totalStars(gameLevel), head(nullptr), timeRemaining(timeLimit) {
    srand(time(nullptr));
    initializeGrid();
    placeStars(gameLevel);
}

void Grid::initializeGrid() {
    Node* prevRow = nullptr;

    for (int x = 0; x < 30; ++x) {
        Node* prevNode = nullptr;
        Node* currentRowHead = nullptr;

        for (int y = 0; y < 30; ++y) {
            Node* newNode = new Node(x, y, false, getValidCandyColor(x, y));

            if (!head) {
                head = newNode;  // Set the head pointer for the grid
            }

            if (!currentRowHead) {
                currentRowHead = newNode;  // Set the head of the current row
            }

            // Link left-right
            if (prevNode) {
                prevNode->right = newNode;
                newNode->left = prevNode;
            }

            // Link up-down
            if (prevRow) {
                Node* aboveNode = prevRow;
                for (int i = 0; i < y; ++i) {
                    aboveNode = aboveNode->right;
                }
                aboveNode->down = newNode;
                newNode->up = aboveNode;
            }

            prevNode = newNode;
        }

        prevRow = currentRowHead;
    }
}

void Grid::updateTimer() {
    while (timeRemaining > 0) {
        this_thread::sleep_for(chrono::seconds(1));
        timeRemaining--;
    }
}

Node* Grid::getNode(int x, int y) {
    if (x < 0 || x >= 30 || y < 0 || y >= 30) return nullptr;
    Node* currentRow = head;
    while (currentRow) {
        Node* current = currentRow;
        while (current) {
            if (current->x == x && current->y == y) {
                return current;
            }
            current = current->right;
        }
        currentRow = currentRow->down;
    }
    return nullptr; // In case no node is found
}

char Grid::getValidCandyColor(int x, int y) {
    char newColor;
    do {
        newColor = candyColors[rand() % 3];

        // Check horizontally (prevent 3 matching candies in a row)
        if (y >= 2 && getNode(x, y - 1)->candyColor == newColor && getNode(x, y - 2)->candyColor == newColor)
            continue;

        // Check vertically (prevent 3 matching candies in a column)
        if (x >= 2 && getNode(x - 1, y)->candyColor == newColor && getNode(x - 2, y)->candyColor == newColor)
            continue;

        break;
    } while (true);

    return newColor;
}

void Grid::placeStars(int gameLevel) {
    set<pair<int, int>> starPositions;

    while (starPositions.size() < gameLevel) {
        int randX = rand() % 30;
        int randY = rand() % 30;

        Node* node = getNode(randX, randY);

        if (node && node->hasStar) continue;

        starPositions.insert({ randX, randY });
    }

    for (const auto& pos : starPositions) {
        int x = pos.first;
        int y = pos.second;

        Node* node = getNode(x, y);
        if (node) {
            node->hasStar = true;
        }
    }
}

void Grid::display() {
    cout << "Time Remaining: " << timeRemaining << " seconds";
    cout << "\nRemaining Moves: " << movesRemaining << "\n";
    cout << "Stars Collected: " << starsCollected << " / " << 5 << "\n";
    cout << "Score: " << score << "\n \n";


    cout << "    ";
    for (int y = 1; y <= 30; y++) {
        cout << setw(2) << setfill('0') << y << "   ";
    }
    cout << endl;


    string dashes = string(30 * 5, '_');
    cout << "   " << dashes << endl;


    Node* currentRow = head;

    int rowNum = 1;
    while (currentRow) {
        Node* current = currentRow;

        cout << setw(2) << setfill('0') << rowNum << "| ";

        for (int y = 1; y <= 30; y++) {
            if (current->hasStar) {

                if (current->candyColor == 'R') {
                    cout << "\033[33m";
                }
                else if (current->candyColor == 'G') {
                    cout << "\033[33m";
                }
                else if (current->candyColor == 'B') {
                    cout << "\033[33m";
                }



                cout << "[" << current->candyColor << "*]";
                cout << "\033[0m";
            }
            else {

                if (current->candyColor == 'R') {
                    cout << "\033[31m";
                }
                else if (current->candyColor == 'G') {
                    cout << "\033[32m";
                }
                else if (current->candyColor == 'B') {
                    cout << "\033[34m";
                }



                cout << "[" << current->candyColor << " ]";
                cout << "\033[0m";
            }


            if (y < 30) {
                cout << " ";
            }
            current = current->right;
        }


        cout << endl;
        currentRow = currentRow->down;
        rowNum++;
    }
}

void Grid::swipe(int x1, int y1, int x2, int y2) {
    if (movesRemaining <= 0) {
        cout << "No moves remaining! Game over.\n";
        return;
    }


    if (x1 < 0 || x1 >= 30 || y1 < 0 || y1 >= 30 ||
        x2 < 0 || x2 >= 30 || y2 < 0 || y2 >= 30) {
        cout << "Invalid coordinates! Out of bounds.\n";
        return;
    }


    Node* node1 = getNode(x1, y1);
    Node* node2 = getNode(x2, y2);


    if (node1->hasStar || node2->hasStar) {
        cout << "Cannot move a cell with a star!\n";
        return;
    }


    if (isValidSwipe(x1, y1, x2, y2)) {

        swap(node1->candyColor, node2->candyColor);


        if (!checkMatch(x1, y1) && !checkMatch(x2, y2)) {

            swap(node1->candyColor, node2->candyColor);
            cout << "No match! Swipe reversed.\n";

            checkGameOver();
        }
        else {

            cout << "Match found!\n";
            removeMatches();
            refillGrid();
            movesRemaining--;
            if (starsCollected >= 5) {
                cout << "Congratulations! You collected all stars and won the game!\n";
                exit(0);
            }
            else {
                checkGameOver();
            }
        }
    }
    else {
        cout << "Invalid swipe! Not adjacent.\n";
    }
}

void Grid::checkGameOver() {
    if (movesRemaining <= 0) {
        cout << "Game over! Out of moves.\n";
        exit(0);
    }

    if (timeRemaining <= 0) {
        cout << "Game over! Time's up.\n";
        exit(0);
    }
}

bool Grid::isValidSwipe(int x1, int y1, int x2, int y2) {
    if (x1 < 0 || x1 >= 30 || y1 < 0 || y1 >= 30 ||
        x2 < 0 || x2 >= 30 || y2 < 0 || y2 >= 30 ||
        getNode(x1, y1)->hasStar || getNode(x2, y2)->hasStar) {
        return false;
    }
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return (dx + dy == 1);
}

bool Grid::checkMatch(int x, int y) {
    Node* node = getNode(x, y);
    if (!node) return false; // If node doesn't exist
    char color = node->candyColor;
    int count = 1;

    // Check horizontally (left and right)
    Node* temp = node->right;
    while (temp && temp->candyColor == color) {
        count++;
        temp = temp->right;
    }
    temp = node->left;
    while (temp && temp->candyColor == color) {
        count++;
        temp = temp->left;
    }

    if (count >= 3) return true; // Match found horizontally

    // Check vertically (up and down)
    count = 1;
    temp = node->down;
    while (temp && temp->candyColor == color) {
        count++;
        temp = temp->down;
    }
    temp = node->up;
    while (temp && temp->candyColor == color) {
        count++;
        temp = temp->up;
    }

    return count >= 3; // Match found vertically
}

void Grid::removeMatches() {
    bool matchFound = false;

    Node* currentRow = head;


    bool toRemove[30][30] = { false };


    while (currentRow) {
        Node* current = currentRow;

        while (current) {
            char color = current->candyColor;
            if (color != ' ') {

                int count = 1;
                Node* temp = current->right;

                while (temp && temp->candyColor == color) {
                    count++;
                    temp = temp->right;
                }

                if (count >= 3) {
                    matchFound = true;
                    Node* mark = current;
                    for (int i = 0; i < count; ++i) {
                        toRemove[mark->x][mark->y] = true;
                        mark = mark->right;
                    }
                }
            }
            current = current->right;
        }
        currentRow = currentRow->down;
    }


    for (int col = 0; col < 30; ++col) {
        Node* currentCol = head;
        for (int i = 0; i < col; ++i) {
            currentCol = currentCol->right;
        }

        while (currentCol) {
            char color = currentCol->candyColor;
            if (color != ' ') {

                int count = 1;
                Node* temp = currentCol->down;

                while (temp && temp->candyColor == color) {
                    count++;
                    temp = temp->down;
                }

                if (count >= 3) {
                    matchFound = true;
                    Node* mark = currentCol;
                    for (int i = 0; i < count; ++i) {
                        toRemove[mark->x][mark->y] = true;
                        mark = mark->down;
                    }
                }
            }
            currentCol = currentCol->down;
        }
    }


    currentRow = head;
    while (currentRow) {
        Node* current = currentRow;
        while (current) {
            if (toRemove[current->x][current->y]) {
                if (current->hasStar) {
                    current->hasStar = false;
                    starsCollected++;
                    score += 50;
                    cout << "Star removed at (" << current->x + 1 << ", " << current->y + 1 << ")!\n";
                }
                else {
                    current->candyColor = ' ';
                    score += 10;
                }
            }
            current = current->right;
        }
        currentRow = currentRow->down;
    }


    if (matchFound) {
        cout << "Matches removed!" << endl;
    }
    else {
        cout << "No matches found!" << endl;
    }
}

void Grid::refillGrid() {
    Node* currentRow = head;

    // Traverse through each row
    int x = 0;  // Row index
    while (currentRow) {
        Node* current = currentRow;
        int y = 0;  // Column index

        // Traverse through each node in the current row
        while (current) {
            if (current->candyColor == ' ') {
                // Pass x and y (row and column indices) instead of Node*
                current->candyColor = getValidCandyColor(x, y);
            }
            current = current->right;
            y++;
        }

        currentRow = currentRow->down;
        x++;
    }
}

Grid::~Grid() {
    Node* currentRow = head;

    // Traverse through each row
    while (currentRow) {
        Node* current = currentRow;

        // Traverse through each node in the current row
        while (current) {
            Node* temp = current;
            current = current->right;
            delete temp;  // Deleting the current node
        }

        currentRow = currentRow->down;
    }
}