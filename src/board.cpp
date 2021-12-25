#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Board.hpp"

int count(int array[], int size, int piece)
{
    int count = 0;
    for (int i = 0; i < size; i++)
        if (array[i] == piece)
            count++;
    return count;
}

Board::Board()
{
    std::fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(board[0][0]), 0);
}

void Board::printBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Board::changePiece(int row, int column, int piece)
{
    if (piece != 0 && piece != 1 && piece != 2)
        return;

    board[row][column] = piece;
    moves++;
}

void Board::clearBoard()
{
    std::fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(board[0][0]), 0);
}

int Board::topMostRow(int column)
{
    for (int i = ROWS - 1; i >= 0; i--)
        if (board[i][column] == 0)
            return i;
    return -1;
}

int Board::checkWin()
{
    int winner = 0;
    // check horizonal
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j <= COLUMNS - 4; j++)
        {
            if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1)
            {
                winning_row = i;
                winning_column = j;
                direction = 0;
                winner = 1;
            }
            else if (board[i][j] == 2 && board[i][j + 1] == 2 && board[i][j + 2] == 2 && board[i][j + 3] == 2)
            {
                winning_row = i;
                winning_column = j;
                direction = 0;
                winner = 2;
            }
        }
    }

    // check vertical
    for (int j = 0; j < COLUMNS; j++)
    {
        for (int i = 0; i <= ROWS - 4; i++)
        {
            if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][j] == 1)
            {
                winning_row = i;
                winning_column = j;
                direction = 1;
                winner = 1;
            }
            else if (board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 2 && board[i + 3][j] == 2)
            {
                winning_row = i;
                winning_column = j;
                direction = 1;
                winner = 2;
            }
        }
    }

    // check diagonal

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (i + 3 < ROWS && j + 3 < COLUMNS)
            {
                if (board[i][j] == 1 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1)
                {
                    winning_row = i;
                    winning_column = j;
                    direction = 2;
                    winner = 1;
                }
                else if (board[i][j] == 2 && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 2)
                {
                    winning_row = i;
                    winning_column = j;
                    direction = 2;
                    winner = 2;
                }
            }
            if (i - 3 >= 0 && j - 3 >= 0)
            {
                if (board[i][j] == 1 && board[i - 1][j - 1] == 1 && board[i - 2][j - 2] == 1 && board[i - 3][j - 3] == 1)
                {
                    winning_row = i;
                    winning_column = j;
                    direction = 3;
                    winner = 1;
                }
                else if (board[i][j] == 2 && board[i - 1][j - 1] == 2 && board[i - 2][j - 2] == 2 && board[i - 3][j - 3] == 2)
                {
                    winning_row = i;
                    winning_column = j;
                    direction = 3;
                    winner = 2;
                }
            }
            if (i + 3 < ROWS && j - 3 >= 0)
            {
                if (board[i][j] == 1 && board[i + 1][j - 1] == 1 && board[i + 2][j - 2] == 1 && board[i + 3][j - 3] == 1)
                {
                    winning_row = i;
                    winning_column = j;
                    direction = 4;
                    winner = 1;
                }
                else if (board[i][j] == 2 && board[i + 1][j - 1] == 2 && board[i + 2][j - 2] == 2 && board[i + 3][j - 3] == 2)
                {
                    winning_row = i;
                    winning_column = j;
                    direction = 4;
                    winner = 2;
                }
            }
            if (i - 3 >= 0 && j + 3 < COLUMNS)
            {
                if (board[i][j] == 1 && board[i - 1][j + 1] == 1 && board[i - 2][j + 2] == 1 && board[i - 3][j + 3] == 1)
                {
                    winning_row = i;
                    winning_column = j;
                    direction = 5;
                    winner = 1;
                }
                else if (board[i][j] == 2 && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2)
                {
                    winning_row = i;
                    winning_column = j;
                    direction = 5;
                    winner = 2;
                }
            }
        }
    }

    // check draw
    bool board_full = true;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (board[i][j] == 0)
                board_full = false;
        }
    }

    if (board_full)
        winner = 3;

    return winner;
}

bool Board::onBoard(int row, int column)
{
    if (row >= 0 && row < ROWS && column >= 0 && column < COLUMNS)
        return true;
    return false;
}