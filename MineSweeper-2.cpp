#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

enum CellType {
    Hidden,
    Open,
    Flagged
};

class MineSweeper {
public:
    int rows;
    int cols;
    int mineCount;

    vector<vector<int>> board;
    vector<vector<CellType>> cellStatus;

    MineSweeper(int _rows, int _cols, int _mineCount)
        : rows(_rows), cols(_cols), board(rows, vector<int>(cols, 0)), cellStatus(rows, vector<CellType>(cols, CellType::Hidden)) {
        mineCount = _mineCount;
    }
};

class MineGeneration {
public:
    MineGeneration(MineSweeper& mineObj) {
        int count = 0;
        while (count < mineObj.mineCount) {
            int x = rand() % mineObj.rows;
            int y = rand() % mineObj.cols;

            if (mineObj.board[x][y] != -1) {
                mineObj.board[x][y] = -1; // Mark as mine
                count++;
            }
        }
    }
};
class NumberCalculation {
public:
    void NumberGen(MineSweeper& mineObj) {
        for (int i = 0; i < mineObj.rows; i++) {
            for (int j = 0; j < mineObj.cols; j++) {
                if (mineObj.board[i][j] != -1) {
                    int count = 0;
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            int nx = i + dx;
                            int ny = j + dy;
                            if (nx >= 0 && nx < mineObj.rows && ny >= 0 && ny < mineObj.cols && mineObj.board[nx][ny] == -1) {
                                count++;
                            }
                        }
                    }
                    mineObj.board[i][j] = count;
                }
            }
        }
    }
};


class BoardCreationClass {
public:
    void CreateBoard(MineSweeper& mineObj,NumberCalculation& numCalObj) {
    	numCalObj.NumberGen(mineObj);
        // Draw Column Indices (upper) Line...
        cout << "  ";
        for (int i = 0; i < mineObj.cols; i++) {
            cout << "  " << i << "  ";
        }
        cout << endl;

        // Draw the rest of the board
        for (int i = 0; i < mineObj.rows; i++) {
            drawHorizontalBorder(mineObj.cols);
            for (int j = 0; j < mineObj.cols; j++) {
                cout << "| ";

                if (mineObj.cellStatus[i][j] == CellType::Hidden) {
                    cout << "H";
                } else if (mineObj.cellStatus[i][j] == CellType::Flagged) {
                    cout << "M";
                } else if (mineObj.board[i][j] == -1) {
                    cout << "*";
                } else {
                    cout << mineObj.board[i][j];
                }

                cout << "  ";
            }
            cout << "|" << endl;
        }

        // Draw the bottom border of the board
        drawHorizontalBorder(mineObj.cols);
    }

    void drawHorizontalBorder(int cols) {
        cout << "+";
        for (int i = 0; i < cols; i++) {
            cout << "----+";
        }
        cout << endl;
    }
};



class GridLogic {
    MineSweeper* mineObj;

public:
    GridLogic(MineSweeper& _mineObj) : mineObj(&_mineObj) {}

    void uncoverGrid(int x, int y) {
        if (x < 0 || x >= mineObj->rows || y < 0 || y >= mineObj->cols || mineObj->cellStatus[x][y] != CellType::Hidden) {
            return;
        }

        mineObj->cellStatus[x][y] = CellType::Open;

        if (mineObj->board[x][y] == 0) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    uncoverGrid(x + dx, y + dy);
                }
            }
        }
    }

    void markGrid(int x, int y) {
        if (x >= 0 && x < mineObj->rows && y >= 0 && y < mineObj->cols && mineObj->cellStatus[x][y] == CellType::Hidden) {
            mineObj->cellStatus[x][y] = CellType::Flagged;
        }
    }
};

class WinnerChecker {
    MineSweeper* mineObj;

public:
    WinnerChecker(MineSweeper& _mineObj) : mineObj(&_mineObj) {}

    bool isWon() {
        for (int i = 0; i < mineObj->rows; i++) {
            for (int j = 0; j < mineObj->cols; j++) {
                if (mineObj->board[i][j] != -1 && mineObj->cellStatus[i][j] != CellType::Open) {
                    return false;
                }
            }
        }
        return true;
    }

    bool isGameOver(int x, int y) {
        if (mineObj->board[x][y] == -1) {
            mineObj->cellStatus[x][y] = CellType::Open;
            return true;
        }
        return false;
    }
};

class PlayGame {
    GridLogic* gridObj;
    MineSweeper* mineObj;
    WinnerChecker* winObj;
    BoardCreationClass* boardObj;
NumberCalculation* numCalObj;
public:
    PlayGame(GridLogic& _gridObj, MineSweeper& _mineObj, WinnerChecker& _winObj, BoardCreationClass& _boardObj,NumberCalculation& _numCalObj)
        : gridObj(&_gridObj), mineObj(&_mineObj), winObj(&_winObj), boardObj(&_boardObj), numCalObj(&_numCalObj) {}

    void start() {
       int x, y;

    cout << "Enter the first cell to uncover (x y): ";
  cin >> x >> y;

// Remove the initial loop that checks for game over

    gridObj->uncoverGrid(x, y);
   gridObj->markGrid(x, y);
boardObj->CreateBoard(*mineObj,*numCalObj);
     while (!winObj->isWon()) {
     	
    cout << "Enter the next cell to uncover (x y): ";
    cin >> x >> y;

    if (x < 0 || x >= mineObj->rows || y < 0 || y >= mineObj->cols) {
        cout << "Invalid input! Try again." << endl;
        continue;
    }

    if (winObj->isGameOver(x, y)) {
        boardObj->CreateBoard(*mineObj,*numCalObj);
        cout << "Game Over! You hit a mine." << endl;
        break;
    } else {
        gridObj->uncoverGrid(x, y);
        boardObj->CreateBoard(*mineObj,*numCalObj);
    }
}

    }//start
    
    
    ~PlayGame(){
    	  delete gridObj;
    delete mineObj;
    delete winObj;
    delete boardObj;
   delete numCalObj;
	}
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Initialize random seed

    MineSweeper mineObj(9, 9, 10);
    NumberCalculation numberCal;
    numberCal.NumberGen(mineObj);

    MineGeneration mineGen(mineObj);

    BoardCreationClass boardCreateClass;
    boardCreateClass.CreateBoard(mineObj,numberCal);

    WinnerChecker winObj(mineObj);
    GridLogic gridObj(mineObj);
    PlayGame playGameObj(gridObj, mineObj, winObj, boardCreateClass,numberCal);
    playGameObj.start();

    return 0;
}

