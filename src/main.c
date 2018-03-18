#include <stdio.h>

#include "modbus.h"
#include "modbus_server.h"
#include "i2cfunc.h"
#include "devices/pca9555.h"

#define MODBUS_PORT 502

static int i2c_adapter;

void handle_write_coils(t_ModbusPacket *packet)
{
	int write_start = packet->data.write_coils.write_start;
	int write_length = packet->data.write_coils.write_length;
	tI2C_Status status = pca9555_write_output_range(i2c_adapter, 0x20, write_start, write_start + write_length, packet->data.write_coils.coils);
	if(status < 0) { printf("  Failed to Write I2C Output Range!\n"); }
	else { printf("  Success: "); print_modbus_packet(packet); }
}

int main(int argc, char const *argv[])
{
	i2c_adapter = i2c_init();
	pca9555_config_output_range(i2c_adapter, 0x20, 0, 4);

	int server_sock = init_modbus_server(MODBUS_PORT);
	register_modbus_handler(0x0f, handle_write_coils);
	run_modbus_server(server_sock);
}