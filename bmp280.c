#include "bmp280.h"

void bmp280_init(struct bmp280_device *bmp280)
{
    char filename[20];
    int ret;

    snprintf(filename, 19, "/dev/i2c-%d", bmp280->i2c_adapter);
    
    bmp280->fd = open(filename, O_RDWR);
    if (bmp280->fd < 0) {
        printf("Error accessing i2c interface.\n");
    }

    if (ioctl(bmp280->fd, I2C_SLAVE, bmp280->i2c_addr) < 0) {
        printf("error accessing sensor.\n");
    } else {
        printf("BMP280 access OK\n");
    }

    // read chip ID
    ret = i2c_smbus_read_byte_data(bmp280->fd, BMP280_CHIP_ID_REG);
    printf("Chip ID: 0x%x\n", ret);

    // read status register
    ret = i2c_smbus_read_byte_data(bmp280->fd, BMP280_STATUS_REG);
    printf("Status: 0x%x\n", ret);
}
