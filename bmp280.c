#include "bmp280.h"

void bmp280_init(struct bmp280_device *bmp280)
{
    char filename[20];
    int ret;

    snprintf(filename, 19, "/dev/i2c-%d", bmp280->i2c_adapter);
    
    bmp280->fd = open(filename, O_RDWR);
    if (bmp280->fd < 0) {
#if BMP280_DEBUG == 1
        printf("Error accessing i2c interface.\n");
#endif
    }

    if (ioctl(bmp280->fd, I2C_SLAVE, bmp280->i2c_addr) < 0) {
#if BMP280_DEBUG == 1
        printf("error accessing sensor.\n");
#endif
    } else {
#if BMP280_DEBUG == 1
        printf("BMP280 access OK\n");
#endif
    }

    // read chip ID
    ret = bmp280_read_reg(bmp280, BMP280_CHIP_ID_REG);
#if BMP280_DEBUG == 1
    printf("Chip ID: 0x%x\n", ret);
#endif

    // read status register
    ret = bmp280_read_reg(bmp280, BMP280_STATUS_REG);
#if BMP280_DEBUG == 1
    printf("Status: 0x%x\n", ret);
#endif

    ret = bmp280_read_reg(bmp280, BMP280_CTRL_MEAS_REG);
    ret |= BMP280_TEMP_OS_MASK & (bmp280->temp_os << BMP280_TEMP_OS_POS);
    ret |= BMP280_PRES_OS_MASK & (bmp280->pres_os << BMP280_PRES_OS_POS);
    bmp280_write_reg(bmp280, BMP280_CTRL_MEAS_REG, ret);

    // read status register
    ret = bmp280_read_reg(bmp280, BMP280_CTRL_MEAS_REG);
#if BMP280_DEBUG == 1
    printf("CTRL Meas: 0x%x\n", ret);
#endif

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

void bmp280_read_raw_values(struct bmp280_device *bmp280, uint8_t reg)
{
    uint8_t raw_values[6];

    // First 3 registers are pressure MSB to XLSB (0xF7 to 0xF9),
    // the next 3 are for temperature (0xFA to 0xFC)
    // 20 bit data
    i2c_smbus_read_i2c_block_data(bmp280->fd, reg, 6, raw_values);

    bmp280->pressure_raw = raw_values[0];
    bmp280->pressure_raw <<= 8;
    bmp280->pressure_raw |= raw_values[1];
    bmp280->pressure_raw <<= 4;
    bmp280->pressure_raw |= (raw_values[2]>>4);
    
    //Register FA to FC are for temperature values
    bmp280->temperature_raw = raw_values[3];
    bmp280->temperature_raw <<= 8;
    bmp280->temperature_raw |= raw_values[4];
    bmp280->temperature_raw <<=4;
    bmp280->temperature_raw |= (raw_values[5]>>4);
}

void bmp280_read_trim_params(struct bmp280_device *bmp280)
{
    // BMP280_DIG_xx is the LSB address.
    
    bmp280->trim_params.dig_T1 = bmp280_read_reg(bmp280, BMP280_DIG_T1+1);
    bmp280->trim_params.dig_T1 <<= 8;
    bmp280->trim_params.dig_T1 |= bmp280_read_reg(bmp280, BMP280_DIG_T1);
    
    bmp280->trim_params.dig_T2 = bmp280_read_reg(bmp280, BMP280_DIG_T2+1);
    bmp280->trim_params.dig_T2 <<= 8;
    bmp280->trim_params.dig_T2 |= bmp280_read_reg(bmp280, BMP280_DIG_T2);
    bmp280->trim_params.dig_T2 = (int16_t)bmp280->trim_params.dig_T2;
        
    bmp280->trim_params.dig_T3 = bmp280_read_reg(bmp280, BMP280_DIG_T3+1);
    bmp280->trim_params.dig_T3 <<= 8;
    bmp280->trim_params.dig_T3 |= bmp280_read_reg(bmp280, BMP280_DIG_T3);
    bmp280->trim_params.dig_T3 = (int16_t)bmp280->trim_params.dig_T3;
        
    bmp280->trim_params.dig_P1 = bmp280_read_reg(bmp280, BMP280_DIG_P1+1);
    bmp280->trim_params.dig_P1 <<= 8;
    bmp280->trim_params.dig_P1 |= bmp280_read_reg(bmp280, BMP280_DIG_P1);
        
    bmp280->trim_params.dig_P2 = bmp280_read_reg(bmp280, BMP280_DIG_P2+1);
    bmp280->trim_params.dig_P2 <<= 8;
    bmp280->trim_params.dig_P2 |= bmp280_read_reg(bmp280, BMP280_DIG_P2);
    bmp280->trim_params.dig_P2 = (int16_t)bmp280->trim_params.dig_P2;
        
    bmp280->trim_params.dig_P3 = bmp280_read_reg(bmp280, BMP280_DIG_P3+1);
    bmp280->trim_params.dig_P3 <<= 8;
    bmp280->trim_params.dig_P3 |= bmp280_read_reg(bmp280, BMP280_DIG_P3);
    bmp280->trim_params.dig_P3 = (int16_t)bmp280->trim_params.dig_P3;
        
    bmp280->trim_params.dig_P4 = bmp280_read_reg(bmp280, BMP280_DIG_P4+1);
    bmp280->trim_params.dig_P4 <<= 8;
    bmp280->trim_params.dig_P4 |= bmp280_read_reg(bmp280, BMP280_DIG_P4);
    bmp280->trim_params.dig_P4 = (int16_t)bmp280->trim_params.dig_P4;
        
    bmp280->trim_params.dig_P5 = bmp280_read_reg(bmp280, BMP280_DIG_P5+1);
    bmp280->trim_params.dig_P5 <<= 8;
    bmp280->trim_params.dig_P5 |= bmp280_read_reg(bmp280, BMP280_DIG_P5);
    bmp280->trim_params.dig_P5 = (int16_t)bmp280->trim_params.dig_P5;
        
    bmp280->trim_params.dig_P6 = bmp280_read_reg(bmp280, BMP280_DIG_P6+1);
    bmp280->trim_params.dig_P6 <<= 8;
    bmp280->trim_params.dig_P6 |= bmp280_read_reg(bmp280, BMP280_DIG_P6);
    bmp280->trim_params.dig_P6 = (int16_t)bmp280->trim_params.dig_P6;
        
    bmp280->trim_params.dig_P7 = bmp280_read_reg(bmp280, BMP280_DIG_P7+1);
    bmp280->trim_params.dig_P7 <<= 8;
    bmp280->trim_params.dig_P7 |= bmp280_read_reg(bmp280, BMP280_DIG_P7);
    bmp280->trim_params.dig_P7 = (int16_t)bmp280->trim_params.dig_P7;
        
    bmp280->trim_params.dig_P8 = bmp280_read_reg(bmp280, BMP280_DIG_P8+1);
    bmp280->trim_params.dig_P8 <<= 8;
    bmp280->trim_params.dig_P8 |= bmp280_read_reg(bmp280, BMP280_DIG_P8);
    bmp280->trim_params.dig_P8 = (int16_t)bmp280->trim_params.dig_P8;
        
    bmp280->trim_params.dig_P9 = bmp280_read_reg(bmp280, BMP280_DIG_P9+1);
    bmp280->trim_params.dig_P9 <<= 8;
    bmp280->trim_params.dig_P9 |= bmp280_read_reg(bmp280, BMP280_DIG_P9);
    bmp280->trim_params.dig_P9 = (int16_t)bmp280->trim_params.dig_P9;
    
#if BMP280_DEBUG == 1
    printf("dig_T1: %d\t%x\n", bmp280->trim_params.dig_T1, bmp280->trim_params.dig_T1);
    printf("dig_T2: %d\t%x\n", bmp280->trim_params.dig_T2, bmp280->trim_params.dig_T2);
    printf("dig_T3: %d\t%x\n", bmp280->trim_params.dig_T3, bmp280->trim_params.dig_T3);

    printf("dig_P1: %d\t%x\n", bmp280->trim_params.dig_P1, bmp280->trim_params.dig_P1);
    printf("dig_P2: %d\t%x\n", bmp280->trim_params.dig_P2, bmp280->trim_params.dig_P2);
    printf("dig_P3: %d\t%x\n", bmp280->trim_params.dig_P3, bmp280->trim_params.dig_P3);
    printf("dig_P4: %d\t%x\n", bmp280->trim_params.dig_P4, bmp280->trim_params.dig_P4);
    printf("dig_P5: %d\t%x\n", bmp280->trim_params.dig_P5, bmp280->trim_params.dig_P5);
    printf("dig_P6: %d\t%x\n", bmp280->trim_params.dig_P6, bmp280->trim_params.dig_P6);
    printf("dig_P7: %d\t%x\n", bmp280->trim_params.dig_P7, bmp280->trim_params.dig_P7);
    printf("dig_P8: %d\t%x\n", bmp280->trim_params.dig_P8, bmp280->trim_params.dig_P8);
    printf("dig_P9: %d\t%x\n", bmp280->trim_params.dig_P9, bmp280->trim_params.dig_P9);
#endif
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

void bmp280_convert_temperature_raw_values(struct bmp280_device *bmp280)
{
    double var1, var2, T;
    var1 = (((double)bmp280->temperature_raw)/16384.0 - ((double)bmp280->trim_params.dig_T1)/1024) * ((double)bmp280->trim_params.dig_T2);
    var2 = ((((double)bmp280->temperature_raw)/131072.0 - ((double)bmp280->trim_params.dig_T1)/8192.0) * (((double)bmp280->temperature_raw)/131072.0 - ((double)bmp280->trim_params.dig_T1)/8192.0)) * ((double)bmp280->trim_params.dig_T3);
    bmp280->t_fine = (int32_t) (var1 + var2);
    T = (var1 + var2) / 5120.0;

    bmp280->temperature_val = T;
}

void bmp280_convert_pressure_raw_values(struct bmp280_device *bmp280)
{
    double var1, var2, p;
    var1 = ((double)bmp280->t_fine/2.0) - 64000.0;
    var2 = var1 * var1 * ((double)bmp280->trim_params.dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)bmp280->trim_params.dig_P5) * 2.0;
    var2 = (var2/4.0) + (((double)bmp280->trim_params.dig_P4) * 65536.0);
    var1 = (((double)bmp280->trim_params.dig_P3) * var1 * var1 / 524288.0 + ((double)bmp280->trim_params.dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 /32768.0) * ((double) bmp280->trim_params.dig_P1);
    if (var1 == 0.0) {
        bmp280->pressure_val = 0;
        return;
    }
    p = 1048576.0 - (double)bmp280->pressure_raw;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)bmp280->trim_params.dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)bmp280->trim_params.dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)bmp280->trim_params.dig_P7)) / 16.0;

    bmp280->pressure_val = p;
}

void bmp280_oneshot_read(struct bmp280_device *bmp280)
{
    bmp280_start_forced_meas(bmp280);
    bmp280_wait_for_meas(bmp280);
    bmp280_read_raw_values(bmp280, BMP280_RAW_VAL_REG_START);
    bmp280_convert_temperature_raw_values(bmp280);
    bmp280_convert_pressure_raw_values(bmp280);
}
