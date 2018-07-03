#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "modbus.h"

unsigned short parse_short(const unsigned char *buffer)
{
	return ((unsigned short)buffer[0] << 8) | buffer[1];
}
void serialize_short(unsigned short *byte)
{
	unsigned short tmp = *byte;
	unsigned char *arr = (void *)byte;
	arr[0] = (tmp >> 8) & 0xff;
	arr[1] = tmp & 0xff;
}

void parse_packet_function(const unsigned char *buffer, t_ModbusPacket *packet)
{
	switch(packet->f_id)
	{
		case 0x01: case 0x02:
			packet->data.read_coils.read_start = parse_short(&buffer[8]);
			packet->data.read_coils.read_length = parse_short(&buffer[10]);
			break;
		case 0x03: case 0x04:
			packet->data.read_registers.read_start = parse_short(&buffer[8]);
			packet->data.read_registers.read_length = parse_short(&buffer[10]);
			break;
		case 0x0f:
			packet->data.write_coils.write_start = parse_short(&buffer[8]);
			packet->data.write_coils.write_length = parse_short(&buffer[10]);
			packet->data.write_coils.byte_length = buffer[12];
			memcpy(packet->data.write_coils.coils, &buffer[13], packet->data.write_coils.byte_length);
			break;
	}
}

t_ModbusPacket* parse_packet(const unsigned char *buffer, unsigned int length)
{
	t_ModbusPacket *packet = malloc(length);
	packet->t_id = parse_short(buffer);
	packet->p_id = parse_short(&buffer[2]);
	packet->length = parse_short(&buffer[4]);
	packet->u_id = buffer[6];
	packet->f_id = buffer[7];

	parse_packet_function(buffer, packet);
	return packet;
}
int serialize_packet(t_ModbusPacket *packet)
{
	unsigned char *ret = (void *)packet;

	int length = packet->length + MODBUS_PACKET_MIN_LENGTH;
	serialize_short(&packet->t_id);
	serialize_short(&packet->p_id);
	serialize_short(&packet->length);

	switch(packet->f_id)
	{
		case 0x0f:
			serialize_short(&packet->data.write_coils.write_start);
			serialize_short(&packet->data.write_coils.write_length);
			break;
	}
	return length;
}
int serialize_response_packet(t_ModbusPacket *packet)
{
	int length = -1;
	switch(packet->f_id)
	{
		case 0x01: case 0x02:
			length = MODBUS_PACKET_READ_MIN_LENGTH + packet->data.read_coils_res.byte_length;
			break;
		case 0x03: case 0x04:
			length = MODBUS_PACKET_READ_MIN_LENGTH + packet->data.read_registers_res.byte_length;
			break;
		case 0x0f:
			length = ModbusWriteCoilsResponsePacket_SIZE;
			break;
	}
	packet->length = length - MODBUS_PACKET_MIN_LENGTH;
	serialize_packet(packet);
	return length;
}

void print_modbus_packet(t_ModbusPacket *pak)
{
	printf("  %s(%d, %d", MODBUS_FUNC_NAMES[pak->f_id], pak->t_id, pak->u_id);
	switch(pak->f_id)
	{
		case 0x01: case 0x02:
			printf(", %d-%d",
				pak->data.read_coils.read_start,
				pak->data.read_coils.read_start + pak->data.read_coils.read_length);
				break;
		case 0x03: case 0x04:
			printf(", %d-%d",
				pak->data.read_registers.read_start,
				pak->data.read_registers.read_start + pak->data.read_registers.read_length);
				break;
		case 0x0f:
			printf(", %d-%d, 0x%x",
				pak->data.write_coils.write_start,
				pak->data.write_coils.write_start + pak->data.write_coils.write_length,
				pak->data.write_coils.coils[0] & 0xff);
				break;
	}
	printf(")\n");
}