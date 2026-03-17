#pragma once

#include "utils/base-types.h"
#include "network/packet.h"

#define CLIENT_DEFAULT_USERNAME "user"

#define CLIENT_MAX_USERNAME_LENGTH 512
#define CLIENT_MAX_CHAT_LENGTH 1024

typedef struct Client {
    struct addrinfo* server_address_info;
    int socket;

    bool connected;
    u32 id;

    char username[CLIENT_MAX_USERNAME_LENGTH];

    char chat[CLIENT_MAX_CHAT_LENGTH];
} Client;

Client* client_create(void);
void client_send_packet(Client* client, Packet packet);
void client_connect(Client* client, const char* ip_address, const char* port);
void client_disconnect(Client* client);
void client_update(Client* client);
void client_destroy(Client* client);
