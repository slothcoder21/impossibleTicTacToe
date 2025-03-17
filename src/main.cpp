#include <iostream> 
#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// Function prototypes
void initGameBoard(char board[3][3]);
void displayBoard(const char board[3][3]);
bool makeMove(char board[3][3], int row, int col, char mark);
bool checkWin(const char board[3][3], char mark);
bool checkTie(const char board[3][3]);
int minimax(char board[3][3], int depth, bool isMaximizing);
pair<int, int> findBestMove(char board[3][3]);
pair<int, int> convertInputToCoordinates(int input);
vector<pair<int, int> > findEmptySpaces(const char board[3][3]);

//Defining maximum scores for evaling possible goal states
#define WIN_SCORE 10
#define TIE_SCORE 0
#define LOSS_SCORE -10
//Defining who is playing with x and o
#define PLAYER_MARK 'O'
#define AI_MARK 'X'
#define EMPTY_SPACE '_'

// Win possibilities
vector<vector<pair<int,int> > > winPossibilities;

// Initialize win possibilities
void initWinPossibilities() {
    // Horizontal wins
    vector<pair<int,int> > row1;
    row1.push_back(make_pair(0,0));
    row1.push_back(make_pair(0,1));
    row1.push_back(make_pair(0,2));
    winPossibilities.push_back(row1);
    
    vector<pair<int,int> > row2;
    row2.push_back(make_pair(1,0));
    row2.push_back(make_pair(1,1));
    row2.push_back(make_pair(1,2));
    winPossibilities.push_back(row2);
    
    vector<pair<int,int> > row3;
    row3.push_back(make_pair(2,0));
    row3.push_back(make_pair(2,1));
    row3.push_back(make_pair(2,2));
    winPossibilities.push_back(row3);
    
    // Vertical wins
    vector<pair<int,int> > col1;
    col1.push_back(make_pair(0,0));
    col1.push_back(make_pair(1,0));
    col1.push_back(make_pair(2,0));
    winPossibilities.push_back(col1);
    
    vector<pair<int,int> > col2;
    col2.push_back(make_pair(0,1));
    col2.push_back(make_pair(1,1));
    col2.push_back(make_pair(2,1));
    winPossibilities.push_back(col2);
    
    vector<pair<int,int> > col3;
    col3.push_back(make_pair(0,2));
    col3.push_back(make_pair(1,2));
    col3.push_back(make_pair(2,2));
    winPossibilities.push_back(col3);
    
    // Diagonal wins
    vector<pair<int,int> > diag1;
    diag1.push_back(make_pair(0,0));
    diag1.push_back(make_pair(1,1));
    diag1.push_back(make_pair(2,2));
    winPossibilities.push_back(diag1);
    
    vector<pair<int,int> > diag2;
    diag2.push_back(make_pair(0,2));
    diag2.push_back(make_pair(1,1));
    diag2.push_back(make_pair(2,0));
    winPossibilities.push_back(diag2);
}

int main()
{
    char board[3][3];
    int playerInput;
    
    // Initialize the win possibilities
    initWinPossibilities();
    
    // Initialize the game board
    initGameBoard(board);
    
    cout << "Welcome to Tic Tac Toe!" << endl;
    cout << "You will be playing against an AI." << endl;
    cout << "You are O, and the AI is X." << endl;
    cout << "Enter a number between 1-9 to make your move:" << endl;
    cout << " 1 | 2 | 3 " << endl;
    cout << "---|---|---" << endl;
    cout << " 4 | 5 | 6 " << endl;
    cout << "---|---|---" << endl;
    cout << " 7 | 8 | 9 " << endl << endl;
    
    displayBoard(board);
    
    // Game loop
    while (true) {
        // Player's turn
        cout << "Your move (1-9): ";
        while (!(cin >> playerInput) || playerInput < 1 || playerInput > 9) {
            cout << "Invalid input. Please enter a number between 1-9: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        pair<int, int> coords = convertInputToCoordinates(playerInput);
        
        // Check if the cell is already occupied
        while (board[coords.first][coords.second] != EMPTY_SPACE) {
            cout << "That position is already taken. Try again: ";
            cin >> playerInput;
            coords = convertInputToCoordinates(playerInput);
        }
        
        // Make player's move
        makeMove(board, coords.first, coords.second, PLAYER_MARK);
        displayBoard(board);
        
        // Check if player won
        if (checkWin(board, PLAYER_MARK)) {
            cout << "Congratulations! You won!" << endl;
            break;
        }
        
        // Check for tie
        if (checkTie(board)) {
            cout << "It's a tie!" << endl;
            break;
        }
        
        cout << "AI is making a move..." << endl;
        
        // AI's turn
        pair<int, int> bestMove = findBestMove(board);
        makeMove(board, bestMove.first, bestMove.second, AI_MARK);
        displayBoard(board);
        
        // Check if AI won
        if (checkWin(board, AI_MARK)) {
            cout << "AI wins! Better luck next time." << endl;
            break;
        }
        
        // Check for tie again
        if (checkTie(board)) {
            cout << "It's a tie!" << endl;
            break;
        }
    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}

// Initialize the game board with empty spaces
void initGameBoard(char board[3][3])
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = EMPTY_SPACE;
        }
    }
}

// Display the current state of the board
void displayBoard(const char board[3][3])
{
    cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << " " << endl;
    cout << "---|---|---" << endl;
    cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << " " << endl;
    cout << "---|---|---" << endl;
    cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << " " << endl;
    cout << endl;
}

// Make a move on the board
bool makeMove(char board[3][3], int row, int col, char mark)
{
    if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != EMPTY_SPACE) {
        return false;
    }
    
    board[row][col] = mark;
    return true;
}

// Check if a player has won
bool checkWin(const char board[3][3], char mark)
{
    for (size_t i = 0; i < winPossibilities.size(); i++) {
        bool isWin = true;
        for (size_t j = 0; j < winPossibilities[i].size(); j++) {
            pair<int, int> pos = winPossibilities[i][j];
            if (board[pos.first][pos.second] != mark) {
                isWin = false;
                break;
            }
        }
        if (isWin) return true;
    }
    return false;
}

// Check if the game is a tie
bool checkTie(const char board[3][3])
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY_SPACE) {
                return false;
            }
        }
    }
    return true;
}

// Find all empty spaces on the board
vector<pair<int, int> > findEmptySpaces(const char board[3][3])
{
    vector<pair<int, int> > emptySpaces;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY_SPACE) {
                emptySpaces.push_back(make_pair(i, j));
            }
        }
    }
    
    return emptySpaces;
}

// Convert user input (1-9) to board coordinates (row, col)
pair<int, int> convertInputToCoordinates(int input)
{
    input--; // Convert to 0-based index
    int row = input / 3;
    int col = input % 3;
    return make_pair(row, col);
}

// Minimax algorithm implementation
int minimax(char board[3][3], int depth, bool isMaximizing)
{
    // Check terminal states
    if (checkWin(board, AI_MARK)) {
        return WIN_SCORE;
    }
    if (checkWin(board, PLAYER_MARK)) {
        return LOSS_SCORE;
    }
    if (checkTie(board)) {
        return TIE_SCORE;
    }
    
    // Find all empty spaces
    vector<pair<int, int> > emptySpaces = findEmptySpaces(board);
    
    if (isMaximizing) {
        int bestScore = -1000;
        for (size_t i = 0; i < emptySpaces.size(); i++) {
            pair<int, int> move = emptySpaces[i];
            // Make the move
            board[move.first][move.second] = AI_MARK;
            
            // Recursively call minimax
            int score = minimax(board, depth + 1, false);
            
            // Undo the move
            board[move.first][move.second] = EMPTY_SPACE;
            
            // Update best score
            if (score > bestScore) {
                bestScore = score;
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (size_t i = 0; i < emptySpaces.size(); i++) {
            pair<int, int> move = emptySpaces[i];
            // Make the move
            board[move.first][move.second] = PLAYER_MARK;
            
            // Recursively call minimax
            int score = minimax(board, depth + 1, true);
            
            // Undo the move
            board[move.first][move.second] = EMPTY_SPACE;
            
            // Update best score
            if (score < bestScore) {
                bestScore = score;
            }
        }
        return bestScore;
    }
}

// Find the best move for the AI using minimax
pair<int, int> findBestMove(char board[3][3])
{
    int bestScore = -1000;
    pair<int, int> bestMove = make_pair(-1, -1);
    
    // Find all empty spaces
    vector<pair<int, int> > emptySpaces = findEmptySpaces(board);
    
    for (size_t i = 0; i < emptySpaces.size(); i++) {
        pair<int, int> move = emptySpaces[i];
        // Make the move
        board[move.first][move.second] = AI_MARK;
        
        // Calculate score using minimax
        int score = minimax(board, 0, false);
        
        // Undo the move
        board[move.first][move.second] = EMPTY_SPACE;
        
        // Update best score and move
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    return bestMove;
}



