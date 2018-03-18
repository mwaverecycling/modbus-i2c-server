

#include "modbus.h"
#include "modbus_server.h"
#include "i2cfunc.h"
#include "devices/pca9555.h"

#define MODBUS_PORT 502

/*
int handle_modbus_packet(t_ModbusPacket *packet, int socket)
{
	print_modbus_packet(packet);
	serialize_packet(packet);
	int status = send(socket, packet, ModbusWriteCoilsResponsePacket_SIZE, 0);
	ERROR_IF(status != ModbusWriteCoilsResponsePacket_SIZE, "Failed to send acknowledgement!")
	return 0;
}

int handle_modbus_connection(int socket)
{
	int msg_size, pak_size;
	char recv_buffer[BUFF_SIZE];
	t_ModbusPacket *packet;
	while((msg_size = recv(socket, recv_buffer, BUFF_SIZE, 0)) > 0)
	{
		printf("  Received %d bytes...", msg_size);
		pak_size = (((unsigned int)recv_buffer[4] << 8) | recv_buffer[5]) + 6;
		if(pak_size > msg_size) {
			printf("  Didn't receive full packet of %d bytes!\n", pak_size);
			return -1;
		}
		printf("packet size %d bytes\n", pak_size);
		packet = parse_packet(recv_buffer, pak_size);
		//printf("Handling function [%d]\n", packet->f_id);
		int status = handle_modbus_packet(packet, socket);
		free(packet);
		if(status < 0) { break; }
	}
	ERROR_IF(msg_size < 0, "Error on recv():")
	return msg_size;
}
*/
/*
void run_server(unsigned short port)
{
	int server_sock = -1, client_sock = -1, status;
	
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	EXIT_IF(server_sock < 0, "socket() failed!")

	struct sockaddr_in server_addr = {0};
	//memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	status = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	EXIT_IF(status < 0, "bind() failed!")

	status = listen(server_sock, 1);
	EXIT_IF(status < 0, "listen() failed!")
	printf("Server[%d] Listening...\n", server_sock);

	struct sockaddr_in client_addr = {0};
	unsigned int client_len = sizeof(client_addr);
	while((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len)) >= 0)
	{
		printf("Connection[%d] from [%s]:\n", client_sock, inet_ntoa(client_addr.sin_addr));
		status = handle_modbus_connection(client_sock);
		close(client_sock);
		printf("  Disconnected.\n");
		if(status < 0) { close(server_sock); exit(1); }
	}
	perror("accept() failed");
	printf("  server_sock: %d\n  client_sock: %d\n  client_len: %d\n", server_sock, client_sock, client_len);
	close(server_sock);
}*/

void handle_write_coils(t_ModbusPacket *packet)
{
	print_modbus_packet(packet);
}

int main(int argc, char const *argv[])
{
	int i2c_adapter = i2c_init();
	pca9555_config_output_range(i2c_adapter, 0x20, 0, 4);

	int server_sock = init_modbus_server(MODBUS_PORT);
	register_modbus_handler(0x0f, handle_write_coils);
	run_modbus_server(server_sock);
}