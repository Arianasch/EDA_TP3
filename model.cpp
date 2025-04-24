/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
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
    Piece opponentPiece = (currentplayer == PLAYER_WHITE) ? PIECE_BLACK : PIECE_WHITE;//al dope, con crear uno ok

    for (int x = 0; x < BOARD_SIZE; x++)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            Square move = { x, y };
  
            if (getBoardPiece(model, move) != PIECE_EMPTY)
            {
                continue; 
            }

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    Square auxiliaryMove = move;
					bool foundOpponent = false;

                    if (i == 0 && j == 0)
                    {
                        continue;
                    }  

                    auxiliaryMove.x += i;
                    auxiliaryMove.y += j;

                    while (isSquareValid(auxiliaryMove) && (getBoardPiece(model, auxiliaryMove) == opponentPiece))
                    {
                        auxiliaryMove.x += i;
                        auxiliaryMove.y += j;
						foundOpponent = true;
                    } 
                    //cdo es igual al oponente, signidica que si alguno de los proximos es una ficha d el, ya esta
                    //cuidado, sali del while pero puede ser o xq no es valido o xq es del oponente 
                    /*if (!isSquareValid(auxiliaryMove))
                    {
                        continue;//que salga y vuelva con otra direccion, ver
                        //ahora debo usar un while para ver si encuentro una del jugador actual: si la encuentro
                        //ya esta: searching=false y movimiento valido lo agrego a la lista
                        //sino, otra direccion y sigo: hay q ver la manera de lograr esrto
                    }
                    do
                    {
                        auxiliaryMove.x += i;
                        auxiliaryMove.y += j;
                    } while (isSquareValid(auxiliaryMove) && (getBoardPiece(model, auxiliaryMove) != colorPiece) && (getBoardPiece(model, auxiliaryMove) != PIECE_EMPTY));
                    //si sale es xq es inv o porque la encontro
                    if (!isSquareValid(auxiliaryMove))
                    {
                        continue;//que salga y vuelva con otra direccion, ver
                    }*/
					if (foundOpponent && isSquareValid(auxiliaryMove) && (getBoardPiece(model, auxiliaryMove) == playerPiece))
                    {
                        validMoves.push_back(move);//agrego a la lista el movimiento
                        j = 2;
                        i = 2;
                    }
                }
            }
        }
    }
}

bool playMove(GameModel &model, Square move)
{
    // Set game piece
    Piece playerPiece =
      (getCurrentPlayer(model) == PLAYER_WHITE)
          ? PIECE_WHITE
          : PIECE_BLACK;
	Piece opponentPiece =
		(getCurrentPlayer(model) == PLAYER_WHITE)
		? PIECE_BLACK
		: PIECE_WHITE;
    /*
    ESTO NO FUNCIONA
    Moves maybeMoves;
    getValidMoves(model, maybeMoves);

    auto it = find(maybeMoves.begin(), maybeMoves.end(), move);      // podriamos ver como mejorar el puntero 
    if (it == maybeMoves.end())
    {
        return false; // Move is not valid
    }
    */
    
    Moves maybeMoves;
    getValidMoves(model, maybeMoves);
    bool isValidMove = false;

    for (const auto& sq : maybeMoves) {
        if (sq.x == move.x && sq.y == move.y) {
            isValidMove = true;
            break;
        }
    }
    if (!isValidMove) {
        return false;
    }


    setBoardPiece(model, move, playerPiece);
    
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }

            Square auxiliaryMove = move;
            bool foundOpponent = false;

			auxiliaryMove.x += i;       
			auxiliaryMove.y += j;

			while (isSquareValid(auxiliaryMove) && 
                   (getBoardPiece(model, auxiliaryMove) == opponentPiece))
            {
                auxiliaryMove.x += i;
                auxiliaryMove.y += j;
				foundOpponent = true;
            }

            if ((getBoardPiece(model, auxiliaryMove) == playerPiece) && foundOpponent)
                // no se si deberia cheque isSquareValid()
            {
                while (auxiliaryMove.x != move.x || auxiliaryMove.y != move.y)
                    //creo que && o || es lo mismo, porque se mueven en conjunto
                    //cuando una llega la otra tambien lo hace
                {
                    auxiliaryMove.x -= i;//***************************
                    auxiliaryMove.y -= j;
                    setBoardPiece(model, auxiliaryMove, playerPiece);
                }
            }
        }

    }
    // Update timer
    double currentTime = GetTime();
    model.playerTime[model.currentPlayer] += currentTime - model.turnTimer;
    model.turnTimer = currentTime;

    // Swap player
    model.currentPlayer =
        (model.currentPlayer == PLAYER_WHITE)
        ? PLAYER_BLACK
        : PLAYER_WHITE;

    // Game over?
    Moves validMoves;
    getValidMoves(model, validMoves);

    if (validMoves.size() == 0)
        model.gameOver = true;

    return true;
}
/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
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
    Player currentplayer = getCurrentPlayer(model);
	Piece colorPiece = (currentplayer == PLAYER_WHITE) ? PIECE_WHITE : PIECE_BLACK;
    //Player opponent = getCurrentPlayer(model) == PLAYER_WHITE? PLAYER_BLACK: PLAYER_WHITE;//al dope, con crear uno ok
    validMoves.clear();

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Square move = { x, y };
  
            if (getBoardPiece(model, move) != PIECE_EMPTY)
            {
                continue; 
            }
            Square auxiliaryMove = { x, y };
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0)
                    {
                        continue;
                    }                      
                    do
                    {
                        auxiliaryMove.x += i;
                        auxiliaryMove.y += j;
                    } while (isSquareValid(auxiliaryMove) && (getBoardPiece(model, auxiliaryMove) == !colorPiece) && (getBoardPiece(model, auxiliaryMove) != PIECE_EMPTY));
                    //cdo es igual al oponente, signidica que si alguno de los proximos es una ficha d el, ya esta
            //cuidado, sali del while pero puede ser o xq no es valido o xq es del oponente 
                    if (!isSquareValid(auxiliaryMove))
                    {
                        continue;//que salga y vuelva con otra direccion, ver
                        //ahora debo usar un while para ver si encuentro una del jugador actual: si la encuentro
                        //ya esta: searching=false y movimiento valido lo agrego a la lista
                        //sino, otra direccion y sigo: hay q ver la manera de lograr esrto
                    }
                    do
                    {
                        auxiliaryMove.x += i;
                        auxiliaryMove.y += j;
                    } while (isSquareValid(auxiliaryMove) && (getBoardPiece(model, auxiliaryMove) != colorPiece) && (getBoardPiece(model, auxiliaryMove) != PIECE_EMPTY));
                    //si sale es xq es inv o porque la encontro
                    if (!isSquareValid(auxiliaryMove))
                    {
                        continue;//que salga y vuelva con otra direccion, ver
                    }
                    else
                    {
                        validMoves.push_back(move);//agrego a la lista el movimiento
                        j = 2;
                        i = 2;
                    }

                }

            }

          
        }
    }
}

bool playMove(GameModel &model, Square move)
{
    // Set game piece
  Piece piece =
      (getCurrentPlayer(model) == PLAYER_WHITE)
          ? PIECE_WHITE
          : PIECE_BLACK;

 // Moves maybeMoves;
 //getValidMoves(model, maybeMoves);
  //auto it = find(maybeMoves.begin(), maybeMoves.end(), move);      // podriamos ver como mejorar el puntero 
 //if (it == maybeMoves.end())
  //{
   //  return false; // Move is not valid
 // }

  setBoardPiece(model, move, piece);

  Square auxiliaryMove = move;
  for (int i = -1; i <= 1; i++)
  {
      for (int j = -1; j <= 1; j++)
      {

          auxiliaryMove = move;
          if (i == 0 && j == 0)
          {
              continue;
          }
          do
          {
              auxiliaryMove.x += i;
              auxiliaryMove.y += j;
          } while (isSquareValid(auxiliaryMove) && (getBoardPiece(model, auxiliaryMove) == !piece) && (getBoardPiece(model, auxiliaryMove) != PIECE_EMPTY));

          if (!isSquareValid(auxiliaryMove))
          {
              continue;
          }
          do
          {
              auxiliaryMove.x -= i;//***************************
              auxiliaryMove.y -= j;
              setBoardPiece(model, auxiliaryMove, piece);
          } while (getBoardPiece(model, auxiliaryMove) != piece);
      }

  }
  // Update timer
  double currentTime = GetTime();
  model.playerTime[model.currentPlayer] += currentTime - model.turnTimer;
  model.turnTimer = currentTime;

  // Swap player
  model.currentPlayer =
      (model.currentPlayer == PLAYER_WHITE)
          ? PLAYER_BLACK
          : PLAYER_WHITE;

  // Game over?
  Moves validMoves;
  getValidMoves(model, validMoves);

  if (validMoves.size() == 0)
      model.gameOver = true;

  return true;
}

