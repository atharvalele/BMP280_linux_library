/*
 * BMP280 Driver Testing File
 *
 * This file is used to test the working of the functions defined
 * in the actual BMP280 library files - bmp280.c & bmp280.h
 */



#include "bmp280.h"

int main(int argc, char**argv)
{
    struct bmp280_device bmp280 = {
        .i2c_adapter = 1,
        .i2c_addr = BMP280_I2C_ADDR_PRIM,
    };

    bmp280_init(&bmp280);

    return 0;
}
