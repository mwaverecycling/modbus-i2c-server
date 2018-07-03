#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "modbus_server.h"



#define BUFF_SIZE 256

static void (*handlers[64])(t_ModbusPacket*) = {0};

void register_modbus_handler(unsigned char func_code, void (*handler)(t_ModbusPacket*))
{
	handlers[func_code] = handler;
}

int handle_modbus_packet(t_ModbusPacket *packet, int socket)
{
	if(handlers[packet->f_id] == NULL) { printf("  Modbus Function %d Unsupported!\n", packet->f_id); }
	else { (*handlers[packet->f_id])(packet); }
	int length = serialize_response_packet(packet);
	if(length < 0) { printf("Error serializing reponse packet! func=%d\n", packet->f_id); return -1; }
	int status = send(socket, packet, length, 0);
	if(status != length) { perror("Failed to send acknowledgement!"); return -1; }
	return 0;
}

int handle_modbus_connection(int socket)
{
	int msg_size, pak_size;
	char recv_buffer[BUFF_SIZE];
	t_ModbusPacket *packet;
	while((msg_size = recv(socket, recv_buffer, BUFF_SIZE, 0)) > 0)
	{
		//printf("  Received %d bytes...", msg_size);
		pak_size = (((unsigned int)recv_buffer[4] << 8) | recv_buffer[5]) + 6;
		if(pak_size > msg_size) {
			printf("  Didn't receive full packet of %d bytes!\n", pak_size);
			return -1;
		}
		//printf("packet size %d bytes\n", pak_size);
		packet = parse_packet(recv_buffer, pak_size);
		//printf("Handling function [%d]\n", packet->f_id);
		int status = handle_modbus_packet(packet, socket);
		free(packet);
		if(status < 0) { break; }
	}
	if(msg_size < 0) { perror("Error on recv():"); }
	return msg_size;
}



int init_modbus_server(unsigned short port)
{
	int server_sock = -1, status;
	
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(server_sock < 0) { return -1; }

	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	status = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(status < 0) { return -1; }

	status = listen(server_sock, 1);
	if(status < 0) { return -1; }
	return server_sock;
}
void run_modbus_server(int server_sock)
{
	int client_sock = -1, status;
	struct sockaddr_in client_addr = {0};
	unsigned int client_len = sizeof(client_addr);

	while((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len)) >= 0)
	{
		printf("Connection[%d] from [%s]:\n", client_sock, inet_ntoa(client_addr.sin_addr));
		status = handle_modbus_connection(client_sock);
		close(client_sock);
		printf("Disconnected.\n");
		if(status < 0) { close(server_sock); exit(1); }
	}
	perror("accept() failed");
	//printf("  server_sock: %d\n  client_sock: %d\n  client_len: %d\n", server_sock, client_sock, client_len);
	close(server_sock);
}