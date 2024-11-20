#include <iostream>
#include <vector>
using namespace std;

void displayBoard(const vector<char> &board);
bool checkWin(const vector<char> &board, char player);
bool checkDraw(const vector<char> &board);

int main()
{
    vector<char> board(9, ' ');
    bool gameRunning = true;
    char currentPlayer = 'X';

    cout << "Welcome to Tic-Tac-Toe!\n";

    while (gameRunning)
    {

        displayBoard(board);

        int move;
        bool validMove = false;
        while (!validMove)
        {
            cout << "Player " << currentPlayer << ", enter your move (1-9): ";
            cin >> move;

            if (move >= 1 && move <= 9 && board[move - 1] == ' ')
            {
                board[move - 1] = currentPlayer;
                validMove = true;
            }
            else
            {
                cout << "Invalid move! Please try again.\n";
            }
        }

        if (checkWin(board, currentPlayer))
        {
            displayBoard(board);
            cout << "Player " << currentPlayer << " wins!\n";
            gameRunning = false;
        }
        else if (checkDraw(board))
        {
            displayBoard(board);
            cout << "It's a draw!\n";
            gameRunning = false;
        }
        else
        {

            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    cout << "Game over. Thanks for playing!\n";
    return 0;
}

void displayBoard(const vector<char> &board)
{
    system("clear");
    for (int i = 0; i < 9; i++)
    {
        cout << (board[i] == ' ' ? to_string(i + 1) : string(1, board[i]));
        if ((i + 1) % 3 == 0)
        {
            cout << "\n";
            if (i < 8)
                cout << "---|---|---\n";
        }
        else
        {
            cout << " | ";
        }
    }
    cout << "\n";
}

bool checkWin(const vector<char> &board, char player)
{
    vector<vector<int>> winConditions = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

    for (const auto &condition : winConditions)
    {
        if (board[condition[0]] == player &&
            board[condition[1]] == player &&
            board[condition[2]] == player)
        {
            return true;
        }
    }
    return false;
}

bool checkDraw(const vector<char> &board)
{
    for (const char &cell : board)
    {
        if (cell == ' ')
            return false;
    }
    return true;
}