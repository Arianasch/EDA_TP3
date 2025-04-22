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
    // To-do: your code goes here...
Player currentplayer = model.currentPlayer;
Player opponent = (model.currentPlayer) == PLAYER_WHITE? PLAYER_BLACK: PLAYER_WHITE;//al dope, con crear uno ok
//bool searching = true;
bool validmove=false;
validMoves.clear();

for (int y = 0; y < BOARD_SIZE; y++)
{
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        Square move = { x, y };
        getBoardPiece(model, move); //aca deberia chequear, si no esta empty sali..hacer

        //while (searching)
       // {
        Square auxiliaryMove = { x, y };
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                    continue;//que pase a la siguiente direccion. obvio ver como lo hacemos

                do
                {
                    auxiliaryMove.x += i;
                    auxiliaryMove.y += j;
                } while (isSquareValid(auxiliaryMove) && getBoardPiece(model, auxiliaryMove) != opponent);
                //cdo es igual al oponente, signidica que si alguno de los proximos es una ficha d el, ya esta
        //cuidado, sali del while pero puede ser o xq no es valido o xq es del oponente 
                if (!isSquareValid(auxiliaryMove))
                    break;//que salga y vuelva con otra direccion, ver
                //ahora debo usar un while para ver si encuentro una del jugador actual: si la encuentro
                //ya esta: searching=false y movimiento valido lo agrego a la lista
                //sino, otra direccion y sigo: hay q ver la manera de lograr esrto
                do
                {
                    auxiliaryMove.x += i;
                    auxiliaryMove.y += j;
                } while (isSquareValid(auxiliaryMove) && getBoardPiece(model, auxiliaryMove) != currentplayer);
                //si sale es xq es inv o porque la encontro
                if (!isSquareValid(auxiliaryMove))
                    break;//que salga y vuelva con otra direccion, ver
                else
                {
                    //searching = false;  //no busques mas, la encontte y q salga 
                    validmove = true; //es un movimiento valido y que salga de todo
                    //podria poner i=2 y j=2 asi sale del for. 
                }
            }
            //	}
                //falta contemplar q pasa si salgo del for y no hallo nada. deberia usar otra variable
            validmove = false;
        }

        //salio del for, si valid move es true: ok la encontre, agrego la psoicion xy a la lista
        if(validmove)
		{
			validMoves.push_back(move);//agrego a la lista el movimiento
			validmove = false; //resetear para la proxima
		}
        // +++ TEST
        // Lists all empty squares...
        //if (getBoardPiece(model, move) == PIECE_EMPTY)
          //  validMoves.push_back(move);
        // --- TEST
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

  setBoardPiece(model, move, piece);

  // To-do: your code goes here...
//chequeo si el movimiento es valido?? antes de hacer set, puedo ver si esta en la lista..
// si lo es, volteo todas las fichas del oponente a mi color. habria q consultar si se asume q el move ya es valido

  //como lo hago??
  // reciclo el codigo anterior y voy por direcciones: en cada direccion intento encontrar la siguiente ficha de mi color
  // y una vez q tengo esa posicion, doy vuelta todas las del otro color q estan entre medio. Esto va para cada
  // direccion posible.chequeo: si la q le sigue es de mi color, chau
  // si no, sigo buscando y una vez q hallo otra de mi color, desde esa posicion hasta mi color en la direccion opuesta voy borrancdo
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
