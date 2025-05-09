/**
 * @brief Implements the Reversi game AI
 * @authors Grigera Candelaria, Schiaffino Ariana
 */

#include <cstdlib>

#include "ai.h"

#include "view.h"

#define MAX_NODOS 10000
#define MAX_DEPTH 10
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

	for (auto move : validMoves)
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
		return checkBoard(model);
	}

	Moves validMoves;
	getValidMoves(model, validMoves);

	if (validMoves.size() == 0)
	{
		// Swap player
		model.currentPlayer =
			(model.currentPlayer == PLAYER_WHITE)
			? PLAYER_BLACK
			: PLAYER_WHITE;

		Moves validMoves;
		getValidMoves(model, validMoves);

		if (validMoves.size() == 0)
			model.gameOver = true;
	}

	++evaluatedNodes;

	if (isMaximizingPlayer)
	{
		int bestValue = MIN_VALUE;
		for (auto move : validMoves)
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
		for (auto move : validMoves)
		{
			GameModel auxModel = model;
			playMove(auxModel, move);
			int currentValue = minimax(auxModel, true, evaluatedNodes, depth - 1, alpha, beta);
			worstValue = std::min(worstValue, currentValue);
			beta = std::min(beta, currentValue);

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

	if (validMoves.size() == 0)
	{
		model.currentPlayer =
			(model.currentPlayer == PLAYER_WHITE)
			? PLAYER_BLACK
			: PLAYER_WHITE;

		Moves validMoves;
		getValidMoves(model, validMoves);

		if (validMoves.size() == 0)
			model.gameOver = true;
	}
	return false;
}

int checkBoard(GameModel& model)
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
			Piece piece = getBoardPiece(model, {row, col});

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
