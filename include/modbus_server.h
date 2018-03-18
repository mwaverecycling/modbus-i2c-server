#ifndef MODBUS_SERVER_H
#define MODBUS_SERVER_H 

#include "modbus.h"

int init_modbus_server(unsigned short port);
void run_modbus_server(int server_sock);
void register_modbus_handler(unsigned char func_code, void (*handler)(t_ModbusPacket*));

#endif