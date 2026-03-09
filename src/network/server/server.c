#include "network/server/server.h"
#include "network/packet.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

Server* server_create(const char* port) {
    Server* server = (Server*) malloc(sizeof(Server));
    if (!server) {
        fprintf(stderr, "[ERROR] [SERVER] Failed to allocate memory for server!\n");
        return NULL;
    }

    struct addrinfo address_hints = {0};
    address_hints.ai_family = AF_UNSPEC; // allows for either IPv4 or IPv6, it doesnt matter to us
    address_hints.ai_socktype = SOCK_DGRAM; // use UDP
    address_hints.ai_flags = AI_PASSIVE; // picks the correct ip

    struct addrinfo* address_info;
    if (getaddrinfo(NULL, port, &address_hints, &address_info) != 0) {
        fprintf(stderr, "[ERROR] [SERVER] Failed to get address infomation!\n");
        free(server);
        return NULL;
    }

    server->socket = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
    if (server->socket == -1) {
        fprintf(stderr, "[ERROR] [SERVER] Failed to create socket!\n");
        freeaddrinfo(address_info);
        free(server);
        return NULL;
    }

    if (bind(server->socket, address_info->ai_addr, address_info->ai_addrlen) == -1) {
        fprintf(stderr, "[ERROR] [SERVER] Failed to bind socket!\n");
        close(server->socket);
        freeaddrinfo(address_info);
        free(server);
        return NULL;
    }

    freeaddrinfo(address_info);

    server->client_handlers_length = 0;
    server->running = true;

    server->packet_queue = packet_queue_create();

    printf("[INFO] [SERVER] Created Server at port: %s\n", port);

    return server;
}

PacketStorage server_receive_packet(Server* server, struct sockaddr_storage* client_address, socklen_t* client_address_length) {
    *client_address_length = sizeof(*client_address);

    PacketStorage result = {0};

    i64 bytes_received = recvfrom(server->socket, result.buffer, sizeof(result.buffer), 0, (struct sockaddr*) client_address, client_address_length);
    if (bytes_received == -1) {
        fprintf(stderr, "[ERROR] [SERVER] Failed to received message from client!\n");
    }
    if (bytes_received < (i64) sizeof(PacketHeader)) {
        fprintf(stderr, "[ERROR] [SERVER] Packet received from client is too small!\n");
    }

    memcpy(&result.packet.header, result.buffer, sizeof(result.packet.header));
    if (result.packet.header.data_size > 0) {
        memcpy(&result.packet.data, result.buffer + sizeof(result.packet.header), result.packet.header.data_size);
    }

    return result;
}

void server_send_packet(Server* server, u32 client_id, Packet packet) {
    if (packet.header.data_size > PACKET_MAX_DATA_SIZE) {
        fprintf(stderr, "[ERROR] [SERVER] Packet data is large than max data size: %d!\n", PACKET_MAX_DATA_SIZE);
        return;
    }

    u8 buffer[sizeof(PacketHeader) + PACKET_MAX_DATA_SIZE];
    memcpy(buffer, &packet.header, sizeof(PacketHeader));
    if (packet.header.data_size > 0) { memcpy(buffer + sizeof(PacketHeader), packet.data, packet.header.data_size); }

    if (sendto(server->socket, &buffer, sizeof(PacketHeader) + packet.header.data_size, 0, (const struct sockaddr*) &server->client_handlers[client_id].address, server->client_handlers[client_id].address_length) == -1) {
        fprintf(stderr, "[ERROR] [SERVER] Failed to send message to client!\n");
    }
}

void server_update(Server* server) {
	struct sockaddr_storage client_address;
	socklen_t client_address_length;

	PacketStorage client_packet = server_receive_packet(server, &client_address, &client_address_length);

    char client_address_string[NI_MAXHOST];
    if (getnameinfo((const struct sockaddr*) &client_address, client_address_length, client_address_string, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) == -1) {
        fprintf(stderr, "[ERROR] [SERVER] Failed to convert client ip into string!\n");
    }

    switch (client_packet.packet.header.type) {
        case PACKET_TYPE_PING: printf("[INFO] [SERVER] [%s] Ping received\n", client_address_string); break;
        case PACKET_TYPE_NEW_CONNECTION: {
            server->client_handlers[server->client_handlers_length] = (ClientHandler) {
            	.address = client_address,
             	.address_length = client_address_length,
              	.connected = false,
               	.id = (u32) server->client_handlers_length
            };

            Packet client_handshake_packet = {
                .header = {
                    .type = PACKET_TYPE_HANDSHAKE,
                    .sender_id = (u32) server->client_handlers_length,
                    .data_size = 0,
                },
                .data = NULL,
            };

            server_send_packet(server, (u32) server->client_handlers_length, client_handshake_packet);

            PacketStorage client_handshake = server_receive_packet(server, &client_address, &client_address_length);

            printf("[INFO] [SERVER] [%s] Connected to new client (id: %lu)\n", client_address_string, server->client_handlers_length);
            server->client_handlers[server->client_handlers_length].connected = true;
            server->client_handlers_length++;
        } break;
        default: break;
    }
}

void server_destroy(Server* server) {
    close(server->socket);
    free(server);
}
