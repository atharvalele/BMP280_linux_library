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

    bmp280_oneshot_read(&bmp280);
    
    printf("Temperature = %f C\n", bmp280.temperature_val);
    printf("Pressure = %f Pa\n", bmp280.pressure_val);

    return 0;
}
