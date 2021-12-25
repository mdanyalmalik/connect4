#pragma once
#include "Board.hpp"

void evaluateWindow(int window[], int size, int piece, int &score); // evalates score of given window

class AI
{
public:
    int scorePosition(Board board, int piece);          // assigns a score to a position
    int pickBestCol(Board board, int piece, int moves); // checks all possible moves and picks best one
};