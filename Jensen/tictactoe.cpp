#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>

using namespace std;

// Function prototypes
void displayBoard(const vector<char> &board, int cursorPos);
bool checkWin(const vector<char> &board, char player);
bool checkDraw(const vector<char> &board);
char getArrowKey();
bool promptRestart();

int main()
{
    bool keepPlaying = true;

    while (keepPlaying)
    {
        vector<char> board(9, ' '); // Initialize a new board
        bool gameRunning = true;
        char currentPlayer = 'X';
        int cursorPos = 0; // Start at the top-left corner

        cout << "Welcome to Tic-Tac-Toe!\n";

        while (gameRunning)
        {
            displayBoard(board, cursorPos);

            bool validMove = false;
            while (!validMove)
            {
                char input = getArrowKey();

                switch (input)
                {
                case 'w': // Up
                    if (cursorPos >= 3)
                        cursorPos -= 3;
                    break;
                case 's': // Down
                    if (cursorPos < 6)
                        cursorPos += 3;
                    break;
                case 'a': // Left
                    if (cursorPos % 3 != 0)
                        cursorPos -= 1;
                    break;
                case 'd': // Right
                    if (cursorPos % 3 != 2)
                        cursorPos += 1;
                    break;
                case '\n': // Enter
                    if (board[cursorPos] == ' ')
                    {
                        board[cursorPos] = currentPlayer;
                        validMove = true;
                    }
                    else
                    {
                        cout << "Cell occupied! Choose another cell.\n";
                    }
                    break;
                }
                displayBoard(board, cursorPos);
            }

            // Check for win or draw
            if (checkWin(board, currentPlayer))
            {
                displayBoard(board, cursorPos);
                cout << "Player " << currentPlayer << " wins!\n";
                gameRunning = false;
            }
            else if (checkDraw(board))
            {
                displayBoard(board, cursorPos);
                cout << "It's a draw!\n";
                gameRunning = false;
            }
            else
            {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }

        // Ask if the players want to restart or exit
        keepPlaying = promptRestart();
    }

    cout << "Thanks for playing! Goodbye.\n";
    return 0;
}

// Function to display the game board
void displayBoard(const vector<char> &board, int cursorPos)
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

// Function to check if a player has won
bool checkWin(const vector<char> &board, char player)
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

// Function to check if the game is a draw
bool checkDraw(const vector<char> &board)
{
    for (char cell : board)
    {
        if (cell == ' ')
            return false;
    }
    return true;
}

// Function to get arrow key input
char getArrowKey()
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); // Get current terminal attributes
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new attributes

    char c;
    read(STDIN_FILENO, &c, 1); // Read a single character

    if (c == '\033')
    { // If it's an escape sequence
        char seq[2];
        read(STDIN_FILENO, &seq, 2); // Read the next two characters
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

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore original terminal attributes
    return c;
}

// Function to prompt for restart
bool promptRestart()
{
    char choice;
    cout << "Do you want to play again? (y/n): ";
    cin >> choice;
    return choice == 'y' || choice == 'Y';
}