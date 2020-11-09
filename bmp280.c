#include "bmp280.h"

void bmp280_init() 
{
    printf("BMP280 Initialization\n");

    int bmp280;
    int i2c_adaptor = 1;
    char filename[20];

    int retval;
    snprintf(filename, 19, "/dev/i2c-%d", i2c_adaptor);

    bmp280 = open(filename, O_RDWR);
    if (bmp280 < 0) {
        printf("Error accessign i2c-interface.\n");
    } 

    if (ioctl(bmp280, I2C_SLAVE, BMP280_I2C_ADDR_PRIM) < 0 ) {
        printf("Error accessing sensor.\n");
    }

    retval = i2c_smbus_read_byte_data(bmp280, BMP280_CHIP_ID_REG);
    printf("Chip ID: 0x%x\n", retval);

    retval = i2c_smbus_read_byte_data(bmp280, BMP280_STATUS_REG);
    printf("Status: 0x%x\n", retval);

    

}