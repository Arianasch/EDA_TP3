/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>

#include "ai.h"
#include "controller.h"

Square getBestMove(GameModel &model)
{
    // To-do: your code goes here...



    // +++ TEST
    // Returns a random valid move...
    Moves validMoves;
    getValidMoves(model, validMoves);

    int index = rand() % validMoves.size();
    return validMoves[index];
    // --- TEST
}
