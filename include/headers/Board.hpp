#pragma once
#include <SDL.h>
#include <SDL_image.h>

int count(int array[], int size, int piece);

class Board
{
public:
    Board();
    void changePiece(int row, int column, int piece); // replaces a value in board
    void clearBoard();                                // resets board
    void printBoard();                                // for debugging purposes
    int topMostRow(int column);                       // returns top most free row of board
    int checkWin();                                   // checks win conditions
    bool onBoard(int row, int column);                // checks if given position is inside board

    int moves = 0;

    static const int ROWS = 6;
    static const int COLUMNS = 7;

    int board[6][7];
    int winning_row;
    int winning_column;
    int direction;
};