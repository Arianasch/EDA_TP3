/**
 * @brief Implements the Reversi game AI
 * @authors Grigera Candelaria, Schiaffino Ariana
 */

#ifndef AI_H
#define AI_H

#include "model.h"

/**
 * @brief Returns the best move for a certain position.
 *
 * @return The best move.
 */
Square getBestMove(GameModel &model);

/**
 * @brief Minimax algorithm with alpha-beta
 * @param model The current game state
 * @param isMaximizingPlayer True if current player is MAX, false if MIN
 * @param evaluatedNodes Counter for number of nodes evaluated (for limiting search)
 * @param depth Current depth in the game tree (for limiting search)
 * @return The evaluated score for this node
 */
static int minimax(GameModel& model, bool isMaximizingPlayer, int& evaluatedNodes, int depth, int alpha, int beta);

/**
 * @brief Checks if the game is over
 * @param model The game state to check
 * @return True if game is over, false otherwise
 */
bool gameOver(GameModel& model);


/**
 * @brief Evaluates the current board state
 * @param model The game state to evaluate
 * @param whoIsPlaying True if evaluating for MAX
 * @return Heuristic score of the board position
 */
int checkBoard(GameModel& model);

#endif
