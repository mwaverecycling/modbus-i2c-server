#ifndef MODBUS_I2C_H
#define MODBUS_I2C_H

#define MODBUS_FUNC_NAMES (const char*[16]){\
"INVALID_FUNCTION[00]",\
"READ_COILS",\
"READ_DISCRETE_INPUTS",\
"READ_HOLDING_REGISTERS",\
"READ_INPUT_REGISTERS",\
"WRITE_COIL",\
"WRITE_HOLDING_REGISTER",\
"READ_EXCEPTION_STATUS",\
"DIAGNOSTIC",\
"INVALID_FUNCTION[09]",\
"INVALID_FUNCTION[0A]",\
"UNSUPPORTED_FUNCTION[0B]",\
"UNSUPPORTED_FUNCTION[0C]",\
"INVALID_FUNCTION[0D]",\
"INVALID_FUNCTION[0E]",\
"WRITE_COILS"}

#define MODBUS_PACKET_MIN_LENGTH 6
#define MODBUS_PACKET_MAX_LENGTH 256
#define MODBUS_PACKET_READ_MIN_LENGTH 9

/* *   FUNCTION 01 : Read Multiple Coils    * */
/* *   FUNCTION 02 : Read Discrete Inputs   * */
struct s_ModbusReadCoilsPacket
{
	unsigned short read_start;
	unsigned short read_length;
};
struct s_ModbusReadCoilsResponsePacket
{
	unsigned char byte_length;
	unsigned char coils[MODBUS_PACKET_MAX_LENGTH - MODBUS_PACKET_READ_MIN_LENGTH];
};

/* *   FUNCTION 03 : Read Holding Registers   * */
/* *   FUNCTION 04 : Read  Input  Registers   * */
struct s_ModbusReadRegistersPacket
{
	unsigned short read_start;
	unsigned short read_length;
};
struct s_ModbusReadRegistersResponsePacket
{
	unsigned char byte_length;
	unsigned char registers[MODBUS_PACKET_MAX_LENGTH - MODBUS_PACKET_READ_MIN_LENGTH];
};

/* *   FUNCTION 15 : Force/Write Multiple Coils   * */
struct s_ModbusWriteCoilsPacket
{
	unsigned short write_start;
	unsigned short write_length;
	unsigned char byte_length;
	unsigned char coils[MODBUS_PACKET_MAX_LENGTH - 13];
};
#define ModbusWriteCoilsResponsePacket_SIZE 12
struct s_ModbusWriteCoilsResponsePacket
{
	unsigned short write_start;
	unsigned short write_length;
};



typedef struct s_ModbusPacket
{
	unsigned short t_id;
	unsigned short p_id;
	unsigned short length;
	unsigned char u_id;
	unsigned char f_id;
	union {
		struct s_ModbusReadCoilsPacket read_coils;
		struct s_ModbusReadCoilsResponsePacket read_coils_res;
		struct s_ModbusReadRegistersPacket read_registers;
		struct s_ModbusReadRegistersResponsePacket read_registers_res;
		struct s_ModbusWriteCoilsPacket write_coils;
		struct s_ModbusWriteCoilsResponsePacket write_coils_res;
	} data;
} t_ModbusPacket;

t_ModbusPacket* parse_packet(const unsigned char *buffer, unsigned int length);
int serialize_packet(t_ModbusPacket *packet);
int serialize_response_packet(t_ModbusPacket *packet);

void print_modbus_packet(t_ModbusPacket *pak);

#endif