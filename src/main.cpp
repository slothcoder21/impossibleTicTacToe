#include <iostream> 
#include <utility>
#include <string>
#include <vector>

using namespace std;

void initGameBoard();
void updateGameBoard(int, int);
void gameResult(int);

//Defining maximum scores for evaling possible goal states
#define win 10000;
#define tie 0;
#define loss -10000;
//Defining who is playing with x and o
#define playerMark '0';
#define aiMark     'X';
#define emptySpace '_';

#define startingDepth 0;

vector<vector<pair<int,int>>> winPossibilities
{
    //horizontal wins
    {make_pair(0,0), make_pair(0,1), make_pair(0,2)},
    {make_pair(1,0), make_pair(1,1), make_pair(1,2)},
    {make_pair(2,0), make_pair(2,1), make_pair(2,2)},

    //vertical wins
    {make_pair(0,0), make_pair(1,0), make_pair(2,0)},
    {make_pair(0,1), make_pair(1,1), make_pair(2,1)},
    {make_pair(0,2), make_pair(1,2), make_pair(2,2)},

    //diagonal wins
    {make_pair(0,0), make_pair(1,1), make_pair(2,2)},
    {make_pair(0,2), make_pair(1,1), make_pair(2,0)}
};

int main()
{
    int num = 0;
    string name = "";
    initGameBoard();
    cout << "Welcome to Tic Tac Toe!" << endl;
    cout << "You will be playing against an AI." << endl;
    cout << "Player 1 please enter a number between 1 and 9" << endl;
    
}

void initGameBoard(char board[3][3])
{
    
    cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << " ";
    cout << "---|---|---" << endl;
    cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << " ";
    cout << "---|---|---" << endl;
    cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << " ";

}



