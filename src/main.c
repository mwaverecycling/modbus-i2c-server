#include <stdio.h>

#include "modbus.h"
#include "modbus_server.h"
#include "i2cfunc.h"
#include "devices/pca9555.h"
#include "devices/ads1115.h"

#define MODBUS_PORT 502

static int i2c_adapter;

void handle_write_coils(t_ModbusPacket *packet)
{
	int write_start = packet->data.write_coils.write_start;
	int write_length = packet->data.write_coils.write_length;
	int i2c_address = packet->u_id;

	tI2C_Status status = pca9555_write_output_range(i2c_adapter, i2c_address, write_start, write_start + write_length, packet->data.write_coils.coils);
	if(status < 0) { printf("  Failed to Write I2C Output Range!\n"); }
	else { printf("  Success: "); print_modbus_packet(packet); }
}
void handle_read_coils(t_ModbusPacket *packet)
{
	int read_start = packet->data.read_coils.read_start;
	int read_length = packet->data.read_coils.read_length;
	int i2c_address = packet->u_id;
	
	unsigned char coil_buffer[2];
	tI2C_Status status = pca9555_read_input(i2c_adapter, i2c_address, coil_buffer);

	coil_buffer[0] = (coil_buffer[0] >> read_start) | (coil_buffer[1] << (8 - read_start));
	coil_buffer[1] = coil_buffer[1] >> read_start;

	if(status < 0) { printf("  Failed to Read I2C Input!\n"); }
	else { printf("  Success: "); print_modbus_packet(packet); }
	packet->data.read_coils_res.byte_length = read_length >> 3;
	packet->data.read_coils_res.coils[0] = coil_buffer[0];
	packet->data.read_coils_res.coils[1] = coil_buffer[1];
}
void handle_read_registers(t_ModbusPacket *packet)
{
	int read_start = packet->data.read_registers.read_start;
	int read_length = packet->data.read_registers.read_length;
	int i2c_address = packet->u_id;
	
	unsigned char coil_buffer[2];
	tI2C_Status status = pca9555_read_input(i2c_adapter, i2c_address, coil_buffer);

	coil_buffer[0] = (coil_buffer[0] >> read_start) | (coil_buffer[1] << (8 - read_start));
	coil_buffer[1] = coil_buffer[1] >> read_start;

	if(status < 0) { printf("  Failed to Read I2C Input!\n"); }
	else { printf("  Success: "); print_modbus_packet(packet); }
	packet->data.read_coils_res.byte_length = read_length >> 3;
	packet->data.read_coils_res.coils[0] = coil_buffer[0];
	packet->data.read_coils_res.coils[1] = coil_buffer[1];
}

int main(int argc, char const *argv[])
{
	i2c_adapter = i2c_init();
	pca9555_config_output_range(i2c_adapter, 0x20, 0, 8);
	pca9555_config_output_range(i2c_adapter, 0x21, 8, 16);

	int server_sock = init_modbus_server(MODBUS_PORT);
	register_modbus_handler(0x01, handle_read_coils);
	register_modbus_handler(0x02, handle_read_coils);
	register_modbus_handler(0x03, handle_read_registers);
	register_modbus_handler(0x04, handle_read_registers);
	register_modbus_handler(0x0f, handle_write_coils);
	run_modbus_server(server_sock);
}