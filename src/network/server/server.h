#pragma once

#include <sys/socket.h>

#include "utils/base-types.h"
#include "network/packet.h"

#define SERVER_MAX_CLIENT_CONNECTIONS 100

typedef struct ClientHandler {
    struct sockaddr_storage address;
    u32 id;
} ClientHandler;

typedef struct Server {
    bool running;

    int socket;

    ClientHandler client_handlers[SERVER_MAX_CLIENT_CONNECTIONS];
    usize client_handlers_length;
} Server;

Server* server_create(const char* port);
void server_send_packet(Server* server, u32 client_id, Packet packet);
void server_update(Server* server);
void server_destroy(Server* server);
