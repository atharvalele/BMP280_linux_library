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
        .mode = BMP280_FORCE,
        .temp_os = BMP280_OS_1X,
        .pres_os = BMP280_OS_1X,
    };

    bmp280_init(&bmp280);

    bmp280_start_forced_meas(&bmp280);

    bmp280_wait_for_meas(&bmp280);

    printf("Meas Done.\n");

    return 0;
}
