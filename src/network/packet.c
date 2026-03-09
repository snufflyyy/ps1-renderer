#include "network/packet.h"

#include <string.h>
#include <stdio.h>

PacketQueue packet_queue_create(void) {
	return (PacketQueue) {0};
}

PacketStorage packet_queue_dequeue(PacketQueue* packet_queue) {
	if (packet_queue->packets_length < 0) { return (PacketStorage) {0}; }

	PacketStorage packet;
	memcpy(&packet, &packet_queue->packets[0], sizeof(packet_queue->packets[0]));

	for (usize i = 1; i < packet_queue->packets_length; i++) {
		packet_queue->packets[i - 1] = packet_queue->packets[i];
	}
	packet_queue->packets_length--;

	return packet;
}

void packet_queue_queue(PacketQueue* packet_queue, PacketStorage packet) {
	if (packet_queue->packets_length >= PACKET_MAX_PACKET_QUEUE_LENGTH) {
		fprintf(stderr, "[ERROR] [PACKET QUEUE] Reached max packet queue length!\n");
		return;
 	}

	memcpy(&packet_queue->packets[packet_queue->packets_length], &packet, sizeof(packet));
	packet_queue->packets_length++;
}
