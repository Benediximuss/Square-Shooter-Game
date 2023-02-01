#include "stdafx.h"
#include "Game.h"

int main()
{

    // Init game engine
    Game game;

    int counter = 0;

    // Game loop
    while (game.isGameRunning())
    {
        // Update
        game.update();

        // Render
        game.render();

        counter++;

        if (counter % 60 == 0)
            std::cout << counter / 60 << std::endl;
    }

    return 0;
}
