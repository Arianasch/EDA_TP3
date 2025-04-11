/**
 * @brief Implements the Reversi game view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <string>

#include "raylib.h"

#include "controller.h"
#include "model.h"

#define GAME_NAME "EDAversi"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define SQUARE_SIZE 80
#define SQUARE_PADDING 1.5F
#define SQUARE_CONTENT_OFFSET (SQUARE_PADDING)
#define SQUARE_CONTENT_SIZE (SQUARE_SIZE - 2 * SQUARE_PADDING)

#define PIECE_CENTER (SQUARE_SIZE / 2)
#define PIECE_RADIUS (SQUARE_SIZE * 80 / 100 / 2)

#define BOARD_X 40
#define BOARD_Y 40
#define BOARD_CONTENT_SIZE (BOARD_SIZE * SQUARE_SIZE)

#define OUTERBORDER_X (BOARD_X - OUTERBORDER_PADDING)
#define OUTERBORDER_Y (BOARD_Y - OUTERBORDER_PADDING)
#define OUTERBORDER_PADDING 40
#define OUTERBORDER_WIDTH 10
#define OUTERBORDER_SIZE (BOARD_CONTENT_SIZE + 2 * OUTERBORDER_PADDING)

#define TITLE_FONT_SIZE 72
#define SUBTITLE_FONT_SIZE 36

#define INFO_CENTERED_X (OUTERBORDER_SIZE + (WINDOW_WIDTH - OUTERBORDER_SIZE) / 2)

#define INFO_TITLE_Y (WINDOW_HEIGHT / 2)

#define INFO_WHITE_SCORE_Y (WINDOW_HEIGHT * 1 / 4 - SUBTITLE_FONT_SIZE / 2)
#define INFO_WHITE_TIME_Y (WINDOW_HEIGHT * 1 / 4 + SUBTITLE_FONT_SIZE / 2)

#define INFO_BLACK_SCORE_Y (WINDOW_HEIGHT * 3 / 4 - SUBTITLE_FONT_SIZE / 2)
#define INFO_BLACK_TIME_Y (WINDOW_HEIGHT * 3 / 4 + SUBTITLE_FONT_SIZE / 2)

#define INFO_BUTTON_WIDTH 280
#define INFO_BUTTON_HEIGHT 64

#define INFO_PLAYBLACK_BUTTON_X INFO_CENTERED_X
#define INFO_PLAYBLACK_BUTTON_Y (WINDOW_HEIGHT * 1 / 8)

#define INFO_PLAYWHITE_BUTTON_X INFO_CENTERED_X
#define INFO_PLAYWHITE_BUTTON_Y (WINDOW_HEIGHT * 7 / 8)

void initView()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);

    SetTargetFPS(60);
}

void freeView()
{
    CloseWindow();
}

/**
 * @brief Draws centered text.
 *
 * @param position The center position for the text.
 * @param fontSize The font size.
 * @param s The string.
 */
static void drawCenteredText(Vector2 position,
                             int fontSize,
                             std::string s)
{
    DrawText(s.c_str(),
             (int)position.x - MeasureText(s.c_str(), fontSize) / 2,
             (int)position.y - fontSize / 2,
             fontSize,
             BROWN);
}

/**
 * @brief Draws a player's score.
 *
 * @param position The center position for the score.
 * @param score The score.
 */
static void drawScore(std::string label,
                      Vector2 position,
                      int score)
{
    std::string s = label + std::to_string(score);

    drawCenteredText(position, SUBTITLE_FONT_SIZE, s);
}

/**
 * @brief Draws a player's timer.
 *
 * @param position The center position for the timer.
 * @param time The number of seconds of the timer.
 */
static void drawTimer(Vector2 position,
                      double time)
{
    int totalSeconds = (int)time;

    int seconds = totalSeconds % 60;
    int minutes = totalSeconds / 60;

    std::string s;

    if (minutes < 10)
        s.append("0");
    s.append(std::to_string(minutes));
    s.append(":");
    if (seconds < 10)
        s.append("0");
    s.append(std::to_string(seconds));

    drawCenteredText(position, SUBTITLE_FONT_SIZE, s);
}

/**
 * @brief Draws a button.
 *
 * @param position The position of the button
 * @param label The text of the button
 */
static void drawButton(Vector2 position,
                       std::string label,
                       Color backgroundColor)
{
    DrawRectangle(position.x - INFO_BUTTON_WIDTH / 2,
                  position.y - INFO_BUTTON_HEIGHT / 2,
                  INFO_BUTTON_WIDTH,
                  INFO_BUTTON_HEIGHT,
                  backgroundColor);

    drawCenteredText({position.x,
                      position.y},
                     SUBTITLE_FONT_SIZE,
                     label.c_str());
}

/**
 * @brief Indicates whether the mouse pointer is over a button.
 *
 * @return true or false.
 */
static bool isMousePointerOverButton(Vector2 position)
{
    Vector2 mousePosition = GetMousePosition();

    return ((mousePosition.x >= (position.x - INFO_BUTTON_WIDTH / 2)) &&
            (mousePosition.x < (position.x + INFO_BUTTON_WIDTH / 2)) &&
            (mousePosition.y >= (position.y - INFO_BUTTON_HEIGHT / 2)) &&
            (mousePosition.y < (position.y + INFO_BUTTON_HEIGHT / 2)));
}

void drawView(GameModel &model)
{
    BeginDrawing();

    ClearBackground(BEIGE);

    DrawRectangle(
        OUTERBORDER_X,
        OUTERBORDER_Y,
        OUTERBORDER_SIZE,
        OUTERBORDER_SIZE,
        BLACK);

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Square square = {x, y};

            Vector2 position = {
                BOARD_X + (float)square.x * SQUARE_SIZE,
                BOARD_Y + (float)square.y * SQUARE_SIZE};

            DrawRectangleRounded(
                {position.x + SQUARE_CONTENT_OFFSET,
                 position.y + SQUARE_CONTENT_OFFSET,
                 SQUARE_CONTENT_SIZE,
                 SQUARE_CONTENT_SIZE},
                0.2F,
                6,
                DARKGREEN);

            Piece piece = getBoardPiece(model, square);

            if (piece != PIECE_EMPTY)
                DrawCircle((int)position.x + PIECE_CENTER,
                           (int)position.y + PIECE_CENTER,
                           PIECE_RADIUS,
                           (piece == PIECE_WHITE) ? WHITE : BLACK);
        }

    drawScore("Black score: ",
              {INFO_CENTERED_X,
               INFO_WHITE_SCORE_Y},
              getScore(model,
                           PLAYER_BLACK));
    drawTimer({INFO_CENTERED_X,
               INFO_WHITE_TIME_Y},
              getTimer(model,
                           PLAYER_BLACK));
    drawCenteredText({INFO_CENTERED_X,
                      INFO_TITLE_Y},
                     TITLE_FONT_SIZE,
                     GAME_NAME);
    drawScore("White score: ",
              {INFO_CENTERED_X,
               INFO_BLACK_SCORE_Y},
              getScore(model,
                           PLAYER_WHITE));
    drawTimer({INFO_CENTERED_X,
               INFO_BLACK_TIME_Y},
              getTimer(model,
                           PLAYER_WHITE));

    if (model.gameOver)
    {
        drawButton({INFO_PLAYBLACK_BUTTON_X,
                    INFO_PLAYBLACK_BUTTON_Y},
                   "Play black",
                   BLACK);

        drawButton({INFO_PLAYWHITE_BUTTON_X,
                    INFO_PLAYWHITE_BUTTON_Y},
                   "Play white",
                   WHITE);
    }

    EndDrawing();
}

Square getSquareOnMousePointer()
{
    Vector2 mousePosition = GetMousePosition();
    Square square = {(int)floor((mousePosition.x - BOARD_X) / SQUARE_SIZE),
                     (int)floor((mousePosition.y - BOARD_Y) / SQUARE_SIZE)};

    if (isSquareValid(square))
        return square;
    else
        return GAME_INVALID_SQUARE;
}

bool isMousePointerOverPlayBlackButton()
{
    return isMousePointerOverButton({INFO_PLAYBLACK_BUTTON_X,
                                     INFO_PLAYBLACK_BUTTON_Y});
}

bool isMousePointerOverPlayWhiteButton()
{
    return isMousePointerOverButton({INFO_PLAYWHITE_BUTTON_X,
                                     INFO_PLAYWHITE_BUTTON_Y});
}
