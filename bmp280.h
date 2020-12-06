/*
 * Bosch BMP280 Sensor Header File
 *
 * This file includes basic register definitions
 * and bitmasks that will be required for interfacing
 * with the sensor.
 *
 * The sensor is connected via I2C
 */


#ifndef _BMP280_H_
#define _BMP280_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// linux i2c-dev includes
#include "linux/i2c.h"
#include "linux/i2c-dev.h"
#include "i2c/smbus.h"
#include "sys/ioctl.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"

/* Debug Switch */
#define BMP280_DEBUG                    0

/* Chip I2C Address */
#define BMP280_I2C_ADDR_PRIM            0x76
#define BMP280_I2C_ADDR_SEC             0x77

/* Register Addresses */
#define BMP280_CHIP_ID_REG              0xD0
#define BMP280_RESET_REG                0xE0
#define BMP280_STATUS_REG               0xF3
#define BMP280_CTRL_MEAS_REG            0xF4
#define BMP280_CONFIG_REG               0xF5
#define BMP280_RAW_VAL_REG_START        0xF7
#define BMP280_PRESS_MSB_REG            0xF7
#define BMP280_PRESS_LSB_REG            0xF8
#define BMP280_PRESS_XLSB_REG           0xF9
#define BMP280_TEMP_MSB_REG             0xFA
#define BMP280_TEMP_LSB_REG             0xFB
#define BMP280_TEMP_XLSB_REG            0xFC
#define BMP280_MEAS_STATUS_MASK         0x08

/* Power Modes */
#define BMP280_MODE_MASK                0x03
#define BMP280_SLEEP                    0x00
#define BMP280_FORCE                    0x01
#define BMP280_NORMAL                   0x03

/* Oversampling Macros */
#define BMP280_OS_NONE                  0x00
#define BMP280_OS_1X                    0x01
#define BMP280_OS_2X                    0x02
#define BMP280_OS_4X                    0x03
#define BMP280_OS_8X                    0x04
#define BMP280_OS_16X                   0x05
#define BMP280_TEMP_OS_MASK             0xE0
#define BMP280_PRES_OS_MASK             0x1C
#define BMP280_TEMP_OS_POS              5
#define BMP280_PRES_OS_POS              2

/* Filter Macros */
#define BMP280_FILTER_OFF               0x00
#define BMP280_FILTER_COEFF_2           0x01
#define BMP280_FILTER_COEFF_4           0x02
#define BMP280_FILTER_COEFF_8           0x03
#define BMP280_FILTER_COEFF_16          0x04

/* Compensation Parameter storage */
#define BMP280_DIG_T1                   0x88
#define BMP280_DIG_T2                   0x8A
#define BMP280_DIG_T3					0x8C
#define BMP280_DIG_P1                   0X8E
#define BMP280_DIG_P2                   0X90
#define BMP280_DIG_P3                   0x92
#define BMP280_DIG_P4                   0x94
#define BMP280_DIG_P5                   0x96
#define BMP280_DIG_P6                   0x98
#define BMP280_DIG_P7                   0x9A
#define BMP280_DIG_P8                   0x9C
#define BMP280_DIG_P9                   0x9E

/* Trimming params struct */
struct bmp280_trim_params {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
};

/* Device Struct */
struct bmp280_device {
    uint8_t i2c_adapter;
    uint8_t i2c_addr;
    uint8_t fd;
    uint8_t mode;
    uint8_t temp_os;
    uint8_t pres_os;
    uint8_t raw_values[6];
    uint32_t temperature_raw;
    uint32_t pressure_raw;
    double temperature_val;
    double pressure_val;
    int32_t t_fine;
    struct bmp280_trim_params trim_params;
};


void bmp280_init(struct bmp280_device *bmp280);
void bmp280_config(struct bmp280_device *bmp280, uint8_t i2c_adapter, uint8_t i2c_addr,
                    uint8_t mode, uint8_t temp_os, uint8_t pres_os);
void bmp280_read_trim_params(struct bmp280_device *bmp280);
void bmp280_write_reg(struct bmp280_device *bmp280, uint8_t reg, uint8_t dat);
uint8_t bmp280_read_reg(struct bmp280_device *bmp280, uint8_t reg);
void bmp280_read_raw_values(struct bmp280_device *bmp280, uint8_t reg);
void bmp280_start_forced_meas(struct bmp280_device *bmp280);
uint8_t bmp280_is_meas_in_progress(struct bmp280_device *bmp280);
void bmp280_wait_for_meas(struct bmp280_device *bmp280);
void bmp280_convert_temperature_raw_values(struct bmp280_device *bmp280);
void bmp280_convert_pressure_raw_values(struct bmp280_device *bmp280);
void bmp280_oneshot_read(struct bmp280_device *bmp280);

#endif
