#pragma once

#include "utils/base-types.h"
#include "network/packet.h"

typedef struct Client {
    struct addrinfo* server_address_info;
    int socket;
    u32 id;
} Client;

Client* client_create(const char* ip_address, const char* port);
void client_send_packet(Client* client, Packet packet);
void client_update(Client* client);
void client_destroy(Client* client);
