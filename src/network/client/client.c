#include "network/client/client.h"

#include <asm-generic/errno.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

#include "network/packet.h"

Client* client_create(void) {
    Client* client = (Client*) malloc(sizeof(Client));
    if (!client) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to allocate memory for client!\n");
        return NULL;
    }

    client->server_address_info = NULL;
    client->connected = false;

    memset(client->chat, 0, CLIENT_MAX_CHAT_LENGTH);
    strncpy(client->username, CLIENT_DEFAULT_USERNAME, CLIENT_MAX_USERNAME_LENGTH);

    printf("[INFO] [CLIENT] Created Client!\n");
    return client;
}

bool client_receive_packet(Client* client, PacketStorage* output_packet) {
	struct sockaddr_storage client_address;
    socklen_t client_address_length = sizeof(struct sockaddr_storage);

    i64 bytes_received = recvfrom(client->socket, output_packet->buffer, sizeof(output_packet->buffer), 0, (struct sockaddr*) &client_address, &client_address_length);
    if (bytes_received == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            return false;
        } else {
            fprintf(stderr, "[ERROR] [CLIENT] Failed to received message from server!\n");
            return false;
        }
    }
    if (bytes_received < (i64) sizeof(PacketHeader)) {
        fprintf(stderr, "[ERROR] [CLIENT] Packet received from client is too small!\n");
        return false;
    }

    memcpy(&output_packet->packet.header, output_packet->buffer, sizeof(output_packet->packet.header));
    output_packet->packet.data = output_packet->buffer + sizeof(PacketHeader);

    return true;
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

void client_connect(Client* client, const char* ip_address, const char* port) {
    if (client->connected) { return; }

    struct addrinfo address_hints = {0};
    address_hints.ai_family = AF_UNSPEC; // allows for either IPv4 or IPv6, it doesnt matter to us
    address_hints.ai_socktype = SOCK_DGRAM; // use UDP

    if (client->server_address_info) { freeaddrinfo(client->server_address_info); }
    if (getaddrinfo(ip_address, port, &address_hints, &client->server_address_info) != 0) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to get address infomation while connecting to server!\n");
        return;
    }

    client->socket = socket(client->server_address_info->ai_family, client->server_address_info->ai_socktype, client->server_address_info->ai_protocol);
    if (client->socket == -1) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to create socket!\n");
        freeaddrinfo(client->server_address_info);
        return;
    }

    int flags = fcntl(client->socket, F_GETFL, 0);
    if (flags == -1) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to get socket flags!\n");
        freeaddrinfo(client->server_address_info);
        return;
    }

    if (fcntl(client->socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        fprintf(stderr, "[ERROR] [CLIENT] Failed to set socket flags!\n");
        freeaddrinfo(client->server_address_info);
        return;
    }

	Packet packet = {
		.header = {
			.type = PACKET_TYPE_NEW_CONNECTION,
			.sender_id = 0, // new connection packet type ignores sender_id
			.data_size = 0,
		},
		.data = NULL,
	};

	client_send_packet(client, packet);

	PacketStorage server_packet;
	client_receive_packet(client, &server_packet);
	while (server_packet.packet.header.type != PACKET_TYPE_HANDSHAKE) {
	    client_receive_packet(client, &server_packet);
	}

	client->id = server_packet.packet.header.sender_id;

	packet = (Packet) {
		.header = {
			.type = PACKET_TYPE_HANDSHAKE,
			.sender_id = client->id,
			.data_size = (u32) strnlen(client->username, CLIENT_MAX_USERNAME_LENGTH),
		},
		.data = client->username,
	};

	client_send_packet(client, packet);

	char server_address_string[NI_MAXHOST] = {0};
    if (getnameinfo((const struct sockaddr*) &client->server_address_info->ai_addr, client->server_address_info->ai_addrlen, server_address_string, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) == -1) {
        fprintf(stderr, "[ERROR] [SERVER] Failed to convert client ip into string!\n");
    }

	printf("[INFO] [CLIENT] Connected to server (id: %u) at %s\n", client->id, server_address_string);
	client->connected = true;
}

void client_disconnect(Client* client) {
    if (!client->connected) { return; }

   	Packet packet = {
		.header = {
			.type = PACKET_TYPE_DISCONNECT,
			.sender_id = client->id,
			.data_size = 0,
		},
		.data = NULL,
	};

	client_send_packet(client, packet);

	client->connected = false;
	client->id = 0;
	memset(client->chat, 0, CLIENT_MAX_CHAT_LENGTH);
}

void client_update(Client* client) {
    if (!client->connected) { return; }

	PacketStorage server_packet;
	if (!client_receive_packet(client, &server_packet)) {
	    return;
	}

	switch (server_packet.packet.header.type) {
	    case PACKET_TYPE_MESSAGE: {
			char message[PACKET_MAX_DATA_SIZE];
			strncpy(message, server_packet.packet.data, server_packet.packet.header.data_size);
			message[server_packet.packet.header.data_size] = '\0';

			usize chat_length = strnlen(client->chat, CLIENT_MAX_CHAT_LENGTH);

			strncat(client->chat, message, (CLIENT_MAX_CHAT_LENGTH - chat_length) - 1);
		} break;
		default: break;
	}
}

void client_destroy(Client* client) {
    freeaddrinfo(client->server_address_info);
    close(client->socket);
    free(client);
}
