#include "game.h"

int main(void) {
	Game game = game_create();

    while (game.running) {
   		game_update(&game);
    	game_draw(&game);
    }

    game_destroy(&game);
    return 0;
}
