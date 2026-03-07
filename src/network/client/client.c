#include "network/client/client.h"

#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

#include "network/packet.h"

Client* client_create(const char* ip_address, const char* port) {
    Client* client = (Client*) malloc(sizeof(Client));
    if (!client) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to allocate memory for client!\n");
        return NULL;
    }

    struct addrinfo address_hints = {0};
    address_hints.ai_family = AF_UNSPEC; // allows for either IPv4 or IPv6, it doesnt matter to us
    address_hints.ai_socktype = SOCK_DGRAM; // use UDP

    if (getaddrinfo(ip_address, port, &address_hints, &client->server_address_info) != 0) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to get address infomation!\n");
        free(client);
        return NULL;
    }

    client->socket = socket(client->server_address_info->ai_family, client->server_address_info->ai_socktype, client->server_address_info->ai_protocol);
    if (client->socket == -1) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to create socket!\n");
        freeaddrinfo(client->server_address_info);
        free(client);
        return NULL;
    }

    printf("[INFO] [CLIENT] Created Client!\n");

    return client;
}

void client_send_packet(Client* client, Packet packet) {
    if (packet.header.data_size > PACKET_MAX_DATA_SIZE) {
        fprintf(stderr, "[ERROR] [CLIENT] Packet data is large than max data size: %d!\n", PACKET_MAX_DATA_SIZE);
        return;
    }

    u8 buffer[sizeof(PacketHeader) + PACKET_MAX_DATA_SIZE];
    memcpy(buffer, &packet.header, sizeof(PacketHeader));
    if (packet.header.data_size > 0) {
        memcpy(buffer + sizeof(PacketHeader), packet.data, packet.header.data_size);
    }

    if (sendto(client->socket, &buffer, sizeof(PacketHeader) + packet.header.data_size, 0, client->server_address_info->ai_addr, client->server_address_info->ai_addrlen) == -1) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to send message to server!\n");
    }
}

void client_update(Client* client) {

}

void client_destroy(Client* client) {
    freeaddrinfo(client->server_address_info);
    close(client->socket);
    free(client);
}
