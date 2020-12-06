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

    ret = i2c_smbus_read_byte_data(bmp280->fd, BMP280_CTRL_MEAS_REG);
    ret |= BMP280_TEMP_OS_MASK & (bmp280->temp_os << BMP280_TEMP_OS_POS);
    ret |= BMP280_PRES_OS_MASK & (bmp280->pres_os << BMP280_PRES_OS_POS);
    bmp280_write_reg(bmp280, BMP280_CTRL_MEAS_REG, ret);

    // read status register
    ret = i2c_smbus_read_byte_data(bmp280->fd, BMP280_CTRL_MEAS_REG);
    printf("CTRL Meas: 0x%x\n", ret);

    bmp280_read_trim_params(bmp280);
}

void bmp280_config(struct bmp280_device *bmp280, uint8_t i2c_adapter, uint8_t i2c_addr,
                    uint8_t mode, uint8_t temp_os, uint8_t pres_os)
{
    bmp280->i2c_adapter = i2c_adapter;
    bmp280->i2c_addr = i2c_addr;
    bmp280->mode = mode;
    bmp280->temp_os = temp_os;
    bmp280->pres_os = pres_os;
}

void bmp280_write_reg(struct bmp280_device *bmp280, uint8_t reg, uint8_t dat)
{
    i2c_smbus_write_byte_data(bmp280->fd, reg, dat);
}

uint8_t bmp280_read_reg(struct bmp280_device *bmp280, uint8_t reg)
{
    return i2c_smbus_read_byte_data(bmp280->fd, reg);
}

void bmp280_read_trim_params(struct bmp280_device *bmp280)
{
    // BMP280_DIG_xx is the LSB address.
    
    bmp280->trim_params.dig_T1 = bmp280_read_reg(bmp280, BMP280_DIG_T1+1);
    bmp280->trim_params.dig_T1 <<= 8;
    bmp280->trim_params.dig_T1 |= bmp280_read_reg(bmp280, BMP280_DIG_T1);
    printf("dig_T1: %d\t%x\n", bmp280->trim_params.dig_T1, bmp280->trim_params.dig_T1);

    bmp280->trim_params.dig_T2 = bmp280_read_reg(bmp280, BMP280_DIG_T2+1);
    bmp280->trim_params.dig_T2 <<= 8;
    bmp280->trim_params.dig_T2 |= bmp280_read_reg(bmp280, BMP280_DIG_T2);
    bmp280->trim_params.dig_T2 = (int16_t)bmp280->trim_params.dig_T2;
    printf("dig_T2: %d\t%x\n", bmp280->trim_params.dig_T2, bmp280->trim_params.dig_T2);

}

void bmp280_start_forced_meas(struct bmp280_device *bmp280)
{
    uint8_t ret;
    ret = bmp280_read_reg(bmp280, BMP280_CTRL_MEAS_REG);
    ret |= BMP280_MODE_MASK & BMP280_FORCE;
    
    bmp280_write_reg(bmp280, BMP280_CTRL_MEAS_REG, ret);
}

uint8_t bmp280_is_meas_in_progress(struct bmp280_device *bmp280)
{
    uint8_t ret;
    
    ret = bmp280_read_reg(bmp280, BMP280_STATUS_REG);
    ret = ret & BMP280_MEAS_STATUS_MASK;

    return ret;
}

void bmp280_wait_for_meas(struct bmp280_device *bmp280)
{
    while (bmp280_is_meas_in_progress(bmp280));
}
