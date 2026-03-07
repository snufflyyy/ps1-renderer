#pragma once

#include "utils/base-types.h"

#define PACKET_MAX_DATA_SIZE 1024

typedef enum PacketType {
    PACKET_TYPE_ERROR,
    PACKET_TYPE_HANDSHAKE,
    PACKET_TYPE_DISCONNECT,
    PACKET_TYPE_PING,
    PACKET_TYPE_MESSAGE,
} PacketType;

typedef struct PacketHeader {
    PacketType type;
    u64 data_size;
} PacketHeader;

typedef struct Packet {
    PacketHeader header;
    void* data;
} Packet;
