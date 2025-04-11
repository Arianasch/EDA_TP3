/**
 * @brief Reversi game
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include "model.h"
#include "view.h"
#include "controller.h"

int main()
{
    GameModel model;

    initModel(model);
    initView();

    while (updateView(model))
        ;

    freeView();
}
