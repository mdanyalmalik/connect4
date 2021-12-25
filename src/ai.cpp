#include "AI.hpp"
#include <iostream>

void evaluateWindow(int window[], int size, int piece, int &score)
{
    int opponent = piece % 2 + 1;

    if (count(window, 4, piece) == 4)
        score += 100;
    else if (count(window, 4, piece) == 3 && count(window, 4, 0) == 1)
        score += 10;
    else if (count(window, 4, piece) == 2 && count(window, 4, 0) == 2)
        score += 5;

    if (count(window, 4, opponent) == 3 && count(window, 4, 0) == 1)
        score -= 80;
}

int AI::scorePosition(Board board, int piece)
{
    int score = 0;

    // center
    int center_array[board.ROWS];
    for (int i = 0; i < board.ROWS; i++)
    {
        center_array[i] = board.board[board.COLUMNS / 2][i];
    }
    int center_count = count(center_array, board.ROWS, piece);
    score += center_count * 6;

    // hor
    for (int i = 0; i < board.ROWS; i++)
    {
        int *row = board.board[i];
        for (int j = 0; j <= board.COLUMNS - 4; j++)
        {
            int window[4] = {row[j], row[j + 1], row[j + 2], row[j + 3]};

            evaluateWindow(window, 4, piece, score);
        }
    }

    // vert
    for (int j = 0; j < board.COLUMNS; j++)
    {
        int column[board.ROWS];
        for (int r = 0; r < board.ROWS; r++)
            column[r] = board.board[r][j];
        for (int i = 0; i <= board.ROWS - 4; i++)
        {
            int window[4] = {column[i], column[i + 1], column[i + 2], column[i + 3]};

            evaluateWindow(window, 4, piece, score);
        }
    }

    // pos diagonal
    for (int i = 0; i <= board.ROWS - 4; i++)
    {
        for (int j = 0; j <= board.COLUMNS - 4; j++)
        {
            int window[4];
            for (int k = 0; k < 4; k++)
            {
                window[k] = {board.board[i + k][j + k]};
            }

            evaluateWindow(window, 4, piece, score);
        }
    }

    // neg diagonal
    for (int i = 0; i <= board.ROWS - 4; i++)
    {
        for (int j = 0; j <= board.COLUMNS - 4; j++)
        {
            int window[4];
            for (int k = 0; k < 4; k++)
            {
                window[k] = {board.board[board.ROWS - i - k][board.COLUMNS - j - k]};
            }

            evaluateWindow(window, 4, piece, score);
        }
    }

    // pos neg
    for (int i = 0; i <= board.ROWS - 4; i++)
    {
        for (int j = 0; j <= board.COLUMNS - 4; j++)
        {
            int window[4];
            for (int k = 0; k < 4; k++)
            {
                window[k] = {board.board[i + k][board.COLUMNS - j - k]};
            }

            evaluateWindow(window, 4, piece, score);
        }
    }

    // neg pos
    for (int i = 0; i <= board.ROWS - 4; i++)
    {
        for (int j = 0; j <= board.COLUMNS - 4; j++)
        {
            int window[4];
            for (int k = 0; k < 4; k++)
            {
                window[k] = {board.board[board.ROWS - i - k][j + k]};
            }

            evaluateWindow(window, 4, piece, score);
        }
    }

    return score;
}

int AI::pickBestCol(Board board, int piece, int moves)
{
    int best_col = rand() % board.COLUMNS;
    int best_score = -1000000;

    for (int c = 0; c < board.COLUMNS; c++)
    {
        int r = board.topMostRow(c);
        if (r >= 0)
        {
            Board board2 = board;
            board2.changePiece(r, c, piece);
            int score = scorePosition(board2, piece);
            if (score > best_score)
            {
                best_score = score;
                best_col = c;
            }
        }
    }
    return best_col;
}
