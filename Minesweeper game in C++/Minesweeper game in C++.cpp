#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

enum class Difficulty {
    BEGINNER,
    INTERMEDIATE,
    ADVANCED
};

constexpr int MAXSIDE = 25;
constexpr int MAXMINES = 99;

class Minesweeper {
private:
    int SIDE;
    int MINES;
    vector<vector<char>> realBoard;
    vector<vector<char>> myBoard;
    vector<vector<bool>> visited;
    vector<pair<int, int>> mines;

public:
    Minesweeper(Difficulty difficulty) {
        switch (difficulty) {
        case Difficulty::BEGINNER:
            SIDE = 9;
            MINES = 10;
            break;
        case Difficulty::INTERMEDIATE:
            SIDE = 16;
            MINES = 40;
            break;
        case Difficulty::ADVANCED:
            SIDE = 24;
            MINES = 99;
            break;
        }
        realBoard.resize(SIDE, vector<char>(SIDE, '-'));
        myBoard.resize(SIDE, vector<char>(SIDE, '-'));
        visited.resize(SIDE, vector<bool>(SIDE, false));
    }

    void placeMines() {
        srand(time(NULL));
        int count = 0;
        while (count < MINES) {
            int x = rand() % SIDE;
            int y = rand() % SIDE;
            if (realBoard[x][y] != '*') {
                realBoard[x][y] = '*';
                mines.push_back({ x, y });
                count++;
            }
        }
    }

    void printBoard() {
        cout << "  ";
        for (int i = 0; i < SIDE; ++i)
            cout << i << " ";
        cout << "\n\n";
        for (int i = 0; i < SIDE; ++i) {
            cout << i << " ";
            for (int j = 0; j < SIDE; ++j)
                cout << myBoard[i][j] << " ";
            cout << "\n";
        }
    }

    bool isValid(int row, int col) {
        return row >= 0 && row < SIDE && col >= 0 && col < SIDE;
    }

    int countAdjacentMines(int row, int col) {
        int count = 0;
        vector<pair<int, int>> directions = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},           {0, 1},
            {1, -1},  {1, 0},  {1, 1}
        };
        for (const auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;
            if (isValid(newRow, newCol) && realBoard[newRow][newCol] == '*')
                count++;
        }
        return count;
    }

    void revealEmpty(int row, int col) {
        if (!isValid(row, col) || visited[row][col] || realBoard[row][col] == '*')
            return;
        visited[row][col] = true;
        int count = countAdjacentMines(row, col);
        myBoard[row][col] = count + '0';
        if (count == 0) {
            vector<pair<int, int>> directions = {
                {-1, -1}, {-1, 0}, {-1, 1},
                {0, -1},           {0, 1},
                {1, -1},  {1, 0},  {1, 1}
            };
            for (const auto& dir : directions)
                revealEmpty(row + dir.first, col + dir.second);
        }
    }

    bool makeMove(int row, int col) {
        if (!isValid(row, col) || visited[row][col]) {
            cout << "Invalid move. Please try again.\n";
            return false;
        }
        if (realBoard[row][col] == '*') {
            for (const auto& mine : mines)
                myBoard[mine.first][mine.second] = '*';
            printBoard();
            cout << "\nYou lost!\n";
            return true;
        }
        revealEmpty(row, col);
        return false;
    }

    bool isGameOver() {
        int nonMineCount = SIDE * SIDE - MINES;
        int uncoveredCount = 0;
        for (int i = 0; i < SIDE; ++i) {
            for (int j = 0; j < SIDE; ++j) {
                if (visited[i][j])
                    uncoveredCount++;
            }
        }
        return uncoveredCount == nonMineCount;
    }
};

int main() {
    cout << "Choose the difficulty level:\n";
    cout << "1. Beginner\n";
    cout << "2. Intermediate\n";
    cout << "3. Advanced\n";

    int choice;
    cin >> choice;

    Difficulty difficulty;
    switch (choice) {
    case 1:
        difficulty = Difficulty::BEGINNER;
        break;
    case 2:
        difficulty = Difficulty::INTERMEDIATE;
        break;
    case 3:
        difficulty = Difficulty::ADVANCED;
        break;
    default:
        cout << "Invalid choice. Exiting...\n";
        return 1;
    }

    Minesweeper game(difficulty);
    game.placeMines();

    while (true) {
        game.printBoard();
        int row, col;
        cout << "Enter your move (row column): ";
        cin >> row >> col;
        if (game.makeMove(row, col) || game.isGameOver()) {
            cout << "Game over!\n";
            break;
        }
    }

    return 0;
}
