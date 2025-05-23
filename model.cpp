/**
 * @brief Implements the Reversi game model
 * @authors Grigera Candelaria, Schiaffino Ariana
 */

#include "raylib.h"

#include "model.h"

void initModel(GameModel &model)
{
    model.gameOver = true;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
}

void startModel(GameModel &model)
{
    model.gameOver = false;

    model.currentPlayer = PLAYER_BLACK;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;
    model.turnTimer = GetTime();

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2 - 1] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = PIECE_BLACK;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = PIECE_BLACK;
}

Player getCurrentPlayer(GameModel &model)
{
    return model.currentPlayer;
}

int getScore(GameModel &model, Player player)
{
    int score = 0;

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (((model.board[y][x] == PIECE_WHITE) &&
                 (player == PLAYER_WHITE)) ||
                ((model.board[y][x] == PIECE_BLACK) &&
                 (player == PLAYER_BLACK)))
                score++;
        }

    return score;
}

double getTimer(GameModel &model, Player player)
{
    double turnTime = 0;

    if (!model.gameOver && (player == model.currentPlayer))
        turnTime = GetTime() - model.turnTimer;

    return model.playerTime[player] + turnTime;
}

Piece getBoardPiece(GameModel &model, Square square)
{
    return model.board[square.y][square.x];
}

void setBoardPiece(GameModel &model, Square square, Piece piece)
{
    model.board[square.y][square.x] = piece;
}

bool isSquareValid(Square square)
{
    return (square.x >= 0) &&
           (square.x < BOARD_SIZE) &&
           (square.y >= 0) &&
           (square.y < BOARD_SIZE);
}


void getValidMoves(GameModel &model, Moves &validMoves)
{
    validMoves.clear();
    Player currentplayer = getCurrentPlayer(model);
	Piece playerPiece = (currentplayer == PLAYER_WHITE) ? PIECE_WHITE : PIECE_BLACK;
    Piece opponentPiece = (currentplayer == PLAYER_WHITE) ? PIECE_BLACK : PIECE_WHITE;
	
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            Square move = {x, y}; 

            if (getBoardPiece(model, move) != PIECE_EMPTY)
            {
                continue; 
            }

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    Square auxiliaryMove = move;
					bool foundOpponent = false;

                    if (dx == 0 && dy == 0)
                    {
                        continue;
                    }  

                    auxiliaryMove.x += dx;
                    auxiliaryMove.y += dy;

                    while (isSquareValid(auxiliaryMove) && 
                          (getBoardPiece(model, auxiliaryMove) == opponentPiece))
                    {
                        auxiliaryMove.x += dx;
                        auxiliaryMove.y += dy;
						foundOpponent = true;
                    } 
                    
		            if (foundOpponent && 
                        isSquareValid(auxiliaryMove) && 
                        (getBoardPiece(model, auxiliaryMove) == playerPiece))
                    {
                        validMoves.push_back(move);
                        dy = 2;
                        dx = 2;
                    }
                }
            }
        }
    }
}

bool playMove(GameModel &model, Square move)
{
    
    Piece playerPiece =(getCurrentPlayer(model) == PLAYER_WHITE)? PIECE_WHITE: PIECE_BLACK;
    Piece opponentPiece =(getCurrentPlayer(model) == PLAYER_WHITE)? PIECE_BLACK: PIECE_WHITE;
    
    Moves maybeMoves;
    getValidMoves(model, maybeMoves);
    bool isValidMove = false;

    for (const auto& coord : maybeMoves)
    {
        if (coord.x == move.x && coord.y == move.y) 
        {
            isValidMove = true;
            break;
        }
    }

    if (!isValidMove) 
    {
        return false;
    }


    setBoardPiece(model, move, playerPiece);
    
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }

            Square auxiliaryMove = move;
            bool foundOpponent = false;

			auxiliaryMove.x += dx;
			auxiliaryMove.y += dy;

			while (isSquareValid(auxiliaryMove) && (getBoardPiece(model, auxiliaryMove) == opponentPiece))
            {
                auxiliaryMove.x += dx;
                auxiliaryMove.y += dy;
				foundOpponent = true;
            }

            if ((getBoardPiece(model, auxiliaryMove) == playerPiece) && foundOpponent)  
            {
                while (auxiliaryMove.x != move.x || auxiliaryMove.y != move.y)
                {
                    auxiliaryMove.x -= dx;
                    auxiliaryMove.y -= dy;
                    setBoardPiece(model, auxiliaryMove, playerPiece);
                }
            }
        }
    }

    double currentTime = GetTime();
    model.playerTime[model.currentPlayer] += currentTime - model.turnTimer;
    model.turnTimer = currentTime;

    model.currentPlayer =
        (model.currentPlayer == PLAYER_WHITE)
        ? PLAYER_BLACK
        : PLAYER_WHITE;

    Moves validGameMov;
    getValidMoves(model, validGameMov);

    if (validGameMov.size() == 0)
    {
        model.currentPlayer =
            (model.currentPlayer == PLAYER_WHITE)
            ? PLAYER_BLACK
            : PLAYER_WHITE;

        Moves validGameMov2;
        getValidMoves(model, validGameMov2);

        if (validGameMov2.size() == 0)
            model.gameOver = true;
    }

    return true;
}
