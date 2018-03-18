#ifndef I2C_FUNC_H
#define I2C_FUNC_H

typedef int tI2C_Status;

// Initialize I2C Adapter, using default file /dev/i2c-2
int i2c_init();
// Initialize I2C Adapter, using custom adapter file
int i2c_init_file(char*);
 
// These functions return -1 on error, otherwise return the number of bytes read/written.
// To perform a 'repeated start' use the i2c_write_read function which can write some
// data and then immediately read data without a stop bit in between.
tI2C_Status i2c_read(int, int, unsigned char*, unsigned int);
tI2C_Status i2c_write(int, int, unsigned char*, unsigned int);
int i2c_close(int);

#endif