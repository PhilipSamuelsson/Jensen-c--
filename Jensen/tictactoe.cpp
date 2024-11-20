#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>

using namespace std;

// Board class to manage the game board
class Board
{
private:
    vector<char> board;

public:
    Board() : board(9, ' ') {}

    void display(int cursorPos)
    {
        system("clear"); // Use "cls" for Windows
        for (int i = 0; i < 9; i++)
        {
            if (i == cursorPos)
            {
                cout << "[" << (board[i] == ' ' ? to_string(i + 1) : string(1, board[i])) << "]";
            }
            else
            {
                cout << " " << (board[i] == ' ' ? to_string(i + 1) : string(1, board[i])) << " ";
            }
            if ((i + 1) % 3 == 0)
            {
                cout << "\n";
                if (i < 8)
                    cout << "---|---|---\n";
            }
            else
            {
                cout << "|";
            }
        }
        cout << "\n";
    }

    bool makeMove(int position, char player)
    {
        if (board[position] == ' ')
        {
            board[position] = player;
            return true;
        }
        return false;
    }

    bool checkWin(char player)
    {
        vector<vector<int>> winConditions = {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};
        for (auto &condition : winConditions)
        {
            if (board[condition[0]] == player && board[condition[1]] == player && board[condition[2]] == player)
            {
                return true;
            }
        }
        return false;
    }

    bool checkDraw()
    {
        for (char cell : board)
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

// Game class to manage game flow
class Game
{
private:
    Board board;
    char currentPlayer;
    int cursorPos;

    char getArrowKey()
    {
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        char c;
        read(STDIN_FILENO, &c, 1);

        if (c == '\033')
        {
            char seq[2];
            read(STDIN_FILENO, &seq, 2);
            if (seq[0] == '[')
            {
                switch (seq[1])
                {
                case 'A':
                    c = 'w';
                    break; // Up
                case 'B':
                    c = 's';
                    break; // Down
                case 'C':
                    c = 'd';
                    break; // Right
                case 'D':
                    c = 'a';
                    break; // Left
                }
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return c;
    }

    bool promptRestart()
    {
        char choice;
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;
        return choice == 'y' || choice == 'Y';
    }

public:
    Game() : currentPlayer('X'), cursorPos(0) {}

    void play()
    {
        bool keepPlaying = true;

        while (keepPlaying)
        {
            bool gameRunning = true;
            cout << "Welcome to Tic-Tac-Toe!\n";

            while (gameRunning)
            {
                board.display(cursorPos);

                bool validMove = false;
                while (!validMove)
                {
                    char input = getArrowKey();

                    switch (input)
                    {
                    case 'w':
                        if (cursorPos >= 3)
                            cursorPos -= 3;
                        break;
                    case 's':
                        if (cursorPos < 6)
                            cursorPos += 3;
                        break;
                    case 'a':
                        if (cursorPos % 3 != 0)
                            cursorPos -= 1;
                        break;
                    case 'd':
                        if (cursorPos % 3 != 2)
                            cursorPos += 1;
                        break;
                    case '\n':
                        if (board.makeMove(cursorPos, currentPlayer))
                        {
                            validMove = true;
                        }
                        else
                        {
                            cout << "Cell occupied! Choose another cell.\n";
                        }
                        break;
                    }
                    board.display(cursorPos);
                }

                if (board.checkWin(currentPlayer))
                {
                    board.display(cursorPos);
                    cout << "Player " << currentPlayer << " wins!\n";
                    gameRunning = false;
                }
                else if (board.checkDraw())
                {
                    board.display(cursorPos);
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
                cursorPos = 0;
            }
        }

        cout << "Thanks for playing! Goodbye.\n";
    }
};

int main()
{
    Game game;
    game.play();
    return 0;
}