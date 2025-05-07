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
		int value = minimax(auxModel ,true, evaluatedNodes, MAX_DEPTH - 1, MIN_VALUE, MAX_DEPTH);
		if (value > bestValue)
		{
			bestValue = value;
			bestMove = move;
		}
	}
	return bestMove;
}


static int minimax(GameModel& model, bool whoIsPlaying, int& evaluatedNodes, int depth, int alpha, int beta)
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

	if (whoIsPlaying)
	{
		int bestValue = MIN_VALUE;
		for (auto move : validMoves)
		{
			GameModel auxModel = model;
			playMove(auxModel, move);
			int value = minimax(auxModel, false, evaluatedNodes, depth - 1, alpha, beta);
			bestValue = std::max(bestValue, value);
			alpha = std::max(alpha, value);  // Actualizamos alpha

			// Podar rama si es posible
			if (beta <= alpha) {
				break;  // No es necesario continuar explorando
			}

			// Verificar si hemos alcanzado el límite de nodos
			if (evaluatedNodes >= MAX_NODOS) {
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
			int value = minimax(auxModel, true, evaluatedNodes, depth - 1, alpha, beta);
			worstValue = std::min(worstValue, value);
			beta = std::min(beta, value);  // Actualizamos beta

			// Podar rama si es posible
			if (beta <= alpha) {
				break;  // No es necesario continuar explorando
			}

			// Verificar si hemos alcanzado el límite de nodos
			if (evaluatedNodes >= MAX_NODOS) {
				break;
			}
		}
		return worstValue;
	}
}

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

int checkBoard(GameModel& model, bool whoIsPlaying)
{
	// Check if the game is over
	if (gameOver(model))
	{
		return 0;
	}
	// Calculate the score for the current player
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
