#include <i2cfunc.h>


#ifndef PCA9555
#define PCA9555

#define PCA9555_PIN_OUTPUT 0
#define PCA9555_PIN_INPUT 1

#define PCA9555_REGISTER_INPUT 0x00
#define PCA9555_REGISTER_OUTPUT 0x02
#define PCA9555_REGISTER_POLARITY 0x04
#define PCA9555_REGISTER_CONFIGURE 0x06


tI2C_Status pca9555_read_input(int adapter, int address, unsigned char data[2]);
int pca9555_read_pin(int adapter, int address, int pin);

tI2C_Status pca9555_write_output(int adapter, int address, unsigned char data[2]);
tI2C_Status pca9555_write_pin(int adapter, int address, int pin, int value);

//void pca9555_write_polarity(int address);
//void pca9555_read_polarity(int address, int pin);

tI2C_Status pca9555_config_read(int adapter, int address, unsigned char data[2]);
tI2C_Status pca9555_config_write(int adapter, int address, unsigned char data[2]);

/** Configures pins [start_pin, end_pin) as output, and the rest as input */
tI2C_Status pca9555_config_output_range(int adapter, int address, int start_pin, int end_pin);
/** Configures pins [start_pin, end_pin) as intput, and the rest as output */
tI2C_Status pca9555_config_input_range(int adapter, int address, int start_pin, int end_pin);

#endif