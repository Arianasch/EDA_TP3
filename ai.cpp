/**
 * @brief Implements the Reversi game AI
 * @authors Grigera Candelaria, Schiaffino Ariana
 */

#include <cstdlib>

#include "ai.h"

#include "controller.h"

#define MAX_NODOS 1000000
#define MAX_DEPTH 100
#define MIN_VALUE -10000000
#define MAX_VALUE 10000000
#define CORNER_BONUS 100
#define PIECE_BONUS 1

Square getBestMove(GameModel &model)
{
    Moves validMoves;
    getValidMoves(model, validMoves);

	int bestValue = MIN_VALUE;		
	Square bestMove = validMoves[0];
	int evaluatedNodes = 0;

	for (auto& move : validMoves)
	{
		drawView(model);
		GameModel auxModel = model;
		playMove(auxModel, move);
		int currentValue = minimax(auxModel ,true, evaluatedNodes, MAX_DEPTH - 1, MIN_VALUE, MAX_VALUE);
		if (currentValue > bestValue)
		{
			bestValue = currentValue;
			bestMove = move;
		}
	}
	return bestMove;
}

static int minimax(GameModel& model, bool isMaximizingPlayer, int& evaluatedNodes, int depth, int alpha, int beta)
{
	if (depth == 0 || evaluatedNodes >= MAX_NODOS || gameOver(model))
	{
		return checkBoard(model, isMaximizingPlayer);
	}

	Moves validMoves;
	getValidMoves(model, validMoves);

	if (validMoves.empty())
	{
		return checkBoard(model, isMaximizingPlayer);
	}

	++evaluatedNodes;

	if (isMaximizingPlayer)
	{
		int bestValue = MIN_VALUE;
		for (auto& move : validMoves)
		{
			GameModel auxModel = model;
			playMove(auxModel, move);
			int currentValue = minimax(auxModel, false, evaluatedNodes, depth - 1, alpha, beta);
			bestValue = std::max(bestValue, currentValue);
			alpha = std::max(alpha, currentValue);

			if (beta <= alpha) 
			{
				break;  
			}

		}
		return bestValue;
	}
	else
	{
		int worstValue = MAX_VALUE;
		for (auto& move : validMoves)
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

bool gameOver (GameModel &model)
{
	if (model.gameOver)
	{
		return true;
	}

	Moves validMoves;
	getValidMoves(model, validMoves);

	if (validMoves.empty())
	{
		model.gameOver = true;
		return true;
	}
	return false;
}

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

	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		for (int col = 0; col < BOARD_SIZE; ++col)
		{
			Piece piece = getBoardPiece(model, {col, row});

			if (piece == playerPiece)
			{
				score += PIECE_BONUS;

				if ((row == 0 || row == BOARD_SIZE - 1) && (col == 0 || col == BOARD_SIZE - 1))
				{
					score += CORNER_BONUS;
				}
			}
			else if (piece == opponentPiece)
			{
				score -= PIECE_BONUS;

				if ((row == 0 || row == BOARD_SIZE - 1) && (col == 0 || col == BOARD_SIZE - 1))
				{
					score -= CORNER_BONUS;
				}
			}
		}
	}
	return score;
}
