#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

enum CellType {
    COVERED,
    HIDDEN,
    FLAGGED
};

class MinesweeperBoard {
private:
    int mineCount;
    int rows;
    int cols;
    vector<vector<int>> board;
    vector<vector<CellType>> cellStatus;

public:
    MinesweeperBoard(int _rows, int _cols, int _mineCount) : rows(_rows), cols(_cols) {
        board.assign(rows, vector<int>(cols, 0));
        cellStatus.assign(rows, vector<CellType>(cols, CellType::COVERED));
        MineGeneration(_mineCount);
        cout << endl << endl;
        cout << "--------------------------------------------------------------------------------------------" << endl;
        NumberTilesFilling();
    } // .ctor...

    void MineGeneration(int mineCount) {
        // Place random mines in the blocks...
        int count = 0;
        while (count < mineCount) {
            int x = rand() % rows;
            int y = rand() % cols;
            if (board[x][y] != -1) {
                board[x][y] = -1; // Mark as mine
                count++;
            }
        }
    }

    void NumberTilesFilling() {
        // Function to calculate the number of adjacent mines for each cell
        // Iterate over the board and update the count for each cell
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] != -1) {
                    // If the cell is not a mine, count the number of adjacent mines
                    int count = 0;
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            int nx = i + dx;
                            int ny = j + dy;
                            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && board[nx][ny] == -1) {
                                count++;
                            }
                        }
                    }
                    board[i][j] = count;
                }
            }
        }
    }

    void drawHorizontalBorder(int cols) {
        cout << "+";
        for (int j = 0; j < cols; j++) {
            cout << "---+";
        }
        cout << endl;
    } // drawHorizontalBorder

    void displayNumbers() {
        // Function to display the numbers on the board
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] != -1) {
                    cout << board[i][j] << " ";
                } else {
                    cout << "* ";
                }
            }
            cout << endl;
        }
    } // displayNumbers

    void MakeTheBoard() {
    // Helper function to draw horizontal borders

    // Draw column indices
    cout << "   ";
    for (int j = 0; j < cols; j++) {
        cout << " " << j << "  ";
    }
    cout << endl;

    // Draw board cells with blocks
    for (int i = 0; i < rows; i++) {
        // Draw top border of the block
        drawHorizontalBorder(cols);

        // Draw row index
        cout << i << " ";

        for (int j = 0; j < cols; j++) {
            // Draw block content
            cout << "| ";

            if (cellStatus[i][j] == CellType::COVERED) {
                cout << "- ";
            } else if (cellStatus[i][j] == CellType::FLAGGED) {
                cout << "M ";
            } else {
                if (board[i][j] == -1) {
                    cout << "* ";
                } else {
                    cout << board[i][j] << " ";
                }
            }
        }

        // Draw right border of the last block in the row
        cout << "|" << endl;
    }

    // Draw bottom border of the last row
    drawHorizontalBorder(cols);
}

bool isWon() {
        // Function to check if the game is won
        // Game is won when all non-mine cells are uncovered
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] != -1 && cellStatus[i][j] != CellType::HIDDEN) {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool isGameOver(int x, int y) {
        // Function to check if the game is over
        // Game is over if the selected cell contains a mine
        if (board[x][y] == -1) {
            cellStatus[x][y] = CellType::HIDDEN;
            return true;
        }
        return false;
    } // isGameOver..

    void uncoverGrid(int x, int y) {
        // Function to uncover the selected cell and its neighbors
        if (x < 0 || x >= rows || y < 0 || y >= cols || cellStatus[x][y] != CellType::COVERED) {
            return;
        }

        cellStatus[x][y] = CellType::HIDDEN;

        if (board[x][y] == 0) {
            // Uncover adjacent cells recursively if the current cell has no adjacent mines
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    uncoverGrid(x + dx, y + dy);
                }
            }
        }
    }

    void markGrid(int x, int y) {
        // Function to mark/unmark a cell as a potential mine
        if (x >= 0 && x < rows && y >= 0 && y < cols && cellStatus[x][y] == CellType::COVERED) {
            cellStatus[x][y] = CellType::FLAGGED;
        }
    }
};

int main() {
    MinesweeperBoard Obj(9, 9, 10);

    cout << "Enter the first cell to uncover (x y): ";
    int x, y;
    cout << "Enter x" << endl;
    cin >> x;
    cout << "Enter Y" << endl;
    cin >> y;

    // Ensure that the first cell does not contain a mine
    while (Obj.isGameOver(x, y)) {
        cout << "Please choose another Cell as the first cell contains a mine" << endl;
        cout << "Enter x" << endl;
        cin >> x;
        cout << "Enter Y" << endl;
        cin >> y;
    }

    Obj.uncoverGrid(x, y);
    Obj.MakeTheBoard();

    while (!Obj.isWon()) {
        cout << "Enter the next cell to uncover (x y): ";
        cin >> x >> y;

        if (x < 0 || x >= 9 || y < 0 || y >= 9) {
            cout << "Invalid input! Try again." << endl;
            continue;
        }

        if (Obj.isGameOver(x, y)) {
            Obj.MakeTheBoard();
            cout << "Game Over! You hit a mine." << endl;
            break;
        } else {
            Obj.uncoverGrid(x, y);
            Obj.MakeTheBoard();
        }
    }
} // main...

