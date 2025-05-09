/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>
#include <queue>

#include "ai.h"

#include "controller.h"

#define MAX_NODOS 1000000
#define MAX_DEPTH 1000
#define MIN_VALUE -10000000
#define MAX_VALUE 10000000

Square getBestMove(GameModel &model)
{
    Moves validMoves;
    getValidMoves(model, validMoves);

	int bestValue = MIN_VALUE;		
	Square bestMove = validMoves[0];

	int evaluatedNodes = 0;

	for (auto move : validMoves)
	{
		GameModel auxModel = model;
		playMove(auxModel, move);
		int realValue = minimax(auxModel ,true, evaluatedNodes, MAX_DEPTH - 1, MIN_VALUE, MAX_DEPTH);
		if (realValue > bestValue)
		{
			bestValue = realValue;
			bestMove = move;
		}
	}
	return bestMove;
}

/**
 * @brief Minimax algorithm with alpha-beta
 * @param model The current game state
 * @param whoIsPlaying True if current player is MAX, false if MIN
 * @param evaluatedNodes Counter for number of nodes evaluated (for limiting search)
 * @param depth Current depth in the game tree
 * @return The evaluated score for this node
 */
static int minimax(GameModel& model, bool isMaximizingPlayer, int& evaluatedNodes, int depth, int alpha, int beta)
{
	if (depth == 0 || evaluatedNodes >= MAX_NODOS || gameOver(model))
	{
		return checkBoard(model, whoIsPlaying);
	}

	Moves validMoves;
	getValidMoves(model, validMoves);

	if (validMoves.empty())
	{
		return checkBoard(model, whoIsPlaying);
	}

	++evaluatedNodes;

	if (isMaximizingPlayer)
	{
		int bestValue = MIN_VALUE;
		for (auto move : validMoves)
		{
			GameModel auxModel = model;
			playMove(auxModel, move);
			int realValue = minimax(auxModel, false, evaluatedNodes, depth - 1, alpha, beta);
			bestValue = std::max(bestValue, realValue);
			alpha = std::max(alpha, realValue);  

			if (beta <= alpha) 
			{
				break;  
			}

			if (evaluatedNodes >= MAX_NODOS) 
			{
				break;
			}
		}
		return bestValue;
	}
	else
	{
		int worstValue = MAX_VALUE;
		for (auto move : validMoves)
		{
			GameModel auxModel = model;
			playMove(auxModel, move);
			int realValue = minimax(auxModel, true, evaluatedNodes, depth - 1, alpha, beta);
			worstValue = std::min(worstValue, realValue);
			beta = std::min(beta, realValue);  

			if (beta <= alpha)
			{
				break;  
			}

			if (evaluatedNodes >= MAX_NODOS)
			{
				break;
			}
		}
		return worstValue;
	}
}

/**
 * @brief Checks if the game is over
 * @param model The game state to check
 * @return True if game is over, false otherwise
 */
bool gameOver (GameModel &model)
{
	Moves validMoves;
	getValidMoves(model, validMoves);
	if (validMoves.empty())
	{
		model.gameOver = true;
		return true;
	}
	return false;
}

/**
 * @brief Evaluates the current board state
 * @param model The game state to evaluate
 * @param whoIsPlaying True if evaluating for MAX 
 * @return Heuristic score of the board position
 */

int checkBoard(GameModel& model, bool whoIsPlaying)
{
	if (gameOver(model))
	{
		return 0;
	}
	int score = 0;

	Player currentplayer = getCurrentPlayer(model);
	Piece playerPiece = (currentplayer == PLAYER_WHITE) ? PIECE_WHITE : PIECE_BLACK;
	Piece opponentPiece = (currentplayer == PLAYER_WHITE) ? PIECE_BLACK : PIECE_WHITE;

	if (whoIsPlaying)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				if (getBoardPiece(model, { j, i }) == playerPiece)
				{
					score++;
				}
				else if (getBoardPiece(model, { j, i }) == opponentPiece)
				{
					score--;
				}
				else {}
			}
		}

	}
	else
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				if (getBoardPiece(model, { j, i }) == playerPiece)
				{
					score--;
				}
				else if (getBoardPiece(model, { j, i }) == opponentPiece)
				{
					score++;
				}
				else {}
			}
		}
	}
	return score;
}
