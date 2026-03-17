#pragma once

#include "utils/base-types.h"

#define PACKET_MAX_DATA_SIZE 1024
#define PACKET_MAX_PACKET_QUEUE_LENGTH 10

typedef enum PacketType {
    PACKET_TYPE_NONE,
    PACKET_TYPE_NEW_CONNECTION,
    PACKET_TYPE_HANDSHAKE,
    PACKET_TYPE_DISCONNECT,
    PACKET_TYPE_PING,
    PACKET_TYPE_MESSAGE,
} PacketType;

typedef struct PacketHeader {
    PacketType type;
    u32 sender_id;
    u32 data_size;
} PacketHeader;

typedef struct Packet {
    PacketHeader header;
    void* data;
} Packet;

typedef struct PacketStorage {
	u8 buffer[sizeof(PacketHeader) + PACKET_MAX_DATA_SIZE];
	Packet packet;
} PacketStorage;

typedef struct PacketQueue {
	PacketStorage packets[PACKET_MAX_PACKET_QUEUE_LENGTH];
	usize packets_length;
} PacketQueue;

PacketQueue packet_queue_create(void);
PacketStorage packet_queue_dequeue(PacketQueue* packet_queue);
void packet_queue_queue(PacketQueue* packet_queue, PacketStorage packet);
