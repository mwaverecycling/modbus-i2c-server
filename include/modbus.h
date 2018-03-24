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

struct s_ModbusReadCoilsPacket
{
	unsigned short read_start;
	unsigned short read_length;
};
#define ModbusReadCoilsResponsePacket_SIZE 9
struct s_ModbusReadCoilsResponsePacket
{
	unsigned char byte_length;
	unsigned char coils[MODBUS_PACKET_MAX_LENGTH - 9];
};



typedef struct s_ModbusPacket
{
	unsigned short t_id;
	unsigned short p_id;
	unsigned short length;
	unsigned char u_id;
	unsigned char f_id;
	union {
		struct s_ModbusWriteCoilsPacket write_coils;
		struct s_ModbusWriteCoilsResponsePacket write_coils_res;
		struct s_ModbusReadCoilsPacket read_coils;
		struct s_ModbusReadCoilsResponsePacket read_coils_res;
	} data;
} t_ModbusPacket;

t_ModbusPacket* parse_packet(const unsigned char *buffer, unsigned int length);
int serialize_packet(t_ModbusPacket *packet);
int serialize_response_packet(t_ModbusPacket *packet);

void print_modbus_packet(t_ModbusPacket *pak);

#endif