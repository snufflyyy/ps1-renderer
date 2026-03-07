#include "game.h"

#include <string.h>

#include "network/server/server.h"

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        const char* argument = "server";
        if (strncmp(argv[1], argument, strlen(argument)) == 0) {
            Server* server = server_create("1126");
            while (server->running) { server_update(server); }
            server_destroy(server);
            return 1;
        }
    }

	Game game = game_create();

    while (game.running) {
   		game_update(&game);
    	game_draw(&game);
    }

    game_destroy(&game);
    return 0;
}
