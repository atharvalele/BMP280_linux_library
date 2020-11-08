/*
 * BMP280 Driver Testing File
 *
 * This file is used to test the working of the functions defined
 * in the actual BMP280 library files - bmp280.c & bmp280.h
 */

#include <stdio.h>
#include <stdlib.h>
// linux i2c-dev includes
#include "linux/i2c.h"
#include "linux/i2c-dev.h"
#include "i2c/smbus.h"
#include "sys/ioctl.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"

#include "bmp280.h"

int main(int argc, char**argv)
{
    printf("BMP280 Testing\n");
    
    int bmp280;
    int i2c_adapter = 1;
    char filename[20];

    int ret ;
    snprintf(filename, 19, "/dev/i2c-%d", i2c_adapter);

    bmp280 = open(filename, O_RDWR);
    if (bmp280 < 0) {
        printf("Error accessing i2c-interface.\n");
        exit(1);
    } else {
        printf("i2c-interface access OK.\n");
    }

    if (ioctl(bmp280, I2C_SLAVE, BMP280_I2C_ADDR_PRIM) < 0) {
        printf("Error accessing sensor.\n");
    } else {
        printf("BMP280 access OK\n");
    }

    // read chip ID
    ret = i2c_smbus_read_byte_data(bmp280, BMP280_CHIP_ID_REG);
    printf("Chip ID: 0x%x\n", ret);

    ret = i2c_smbus_read_byte_data(bmp280, BMP280_STATUS_REG);
    printf("Status: 0x%x\n", ret);

    return 0;
}
