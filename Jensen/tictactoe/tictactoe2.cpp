#include <iostream>
#include <vector>
using namespace std;

class Board
{
private:
    vector<char> board;

public:
    Board() : board(9, ' ') {}

    void display()
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

    bool makeMove(int position, char player)
    {
        if (position >= 1 && position <= 9 && board[position - 1] == ' ')
        {
            board[position - 1] = player;
            return true;
        }
        return false;
    }

    bool checkWin(char player)
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

    bool checkDraw()
    {
        for (const char &cell : board)
        {
            if (cell == ' ')
                return false;
        }
        return true;
    }

    void reset()
    {
        board.assign(9, ' ');
    }
};

class Game
{
private:
    Board board;
    char currentPlayer;

    bool promptRestart()
    {
        char choice;
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;
        return (choice == 'y' || choice == 'Y');
    }

public:
    Game() : currentPlayer('X') {}

    void play()
    {
        bool keepPlaying = true;

        while (keepPlaying)
        {
            bool gameRunning = true;
            cout << "Welcome to Tic-Tac-Toe!\n";

            while (gameRunning)
            {
                board.display();

                int move;
                bool validMove = false;
                while (!validMove)
                {
                    cout << "Player " << currentPlayer << ", enter your move (1-9): ";
                    cin >> move;
                    validMove = board.makeMove(move, currentPlayer);
                    if (!validMove)
                    {
                        cout << "Invalid move! Try again.\n";
                    }
                }

                if (board.checkWin(currentPlayer))
                {
                    board.display();
                    cout << "Player " << currentPlayer << " wins!\n";
                    gameRunning = false;
                }
                else if (board.checkDraw())
                {
                    board.display();
                    cout << "It's a draw!\n";
                    gameRunning = false;
                }
                else
                {
                    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                }
            }

            keepPlaying = promptRestart();
            if (keepPlaying)
            {
                board.reset();
                currentPlayer = 'X';
            }
        }

        cout << "Game over. Thanks for playing!\n";
    }
};

int main()
{
    Game game;
    game.play();
    return 0;
}