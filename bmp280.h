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

/* Chip I2C Address */
#define BMP280_I2C_ADDR_PRIM            0x76
#define BMP280_I2C_ADDR_SEC             0x77

/* Register Addresses */
#define BMP280_CHIP_ID_REG              0xD0
#define BMP280_RESET_REG                0xE0
#define BMP280_STATUS_REG               0xF3
#define BMP280_CTRL_MEAS_REG            0xF4
#define BMP280_CONFIG_REG               0xF5
#define BMP280_PRESS_MSB_REG            0xF7
#define BMP280_PRESS_LSB_REG            0xF8
#define BMP280_PRESS_XLSB_REG           0xF9
#define BMP280_TEMP_MSB_REG             0xFA
#define BMP280_TEMP_LSB_REG             0xFB
#define BMP280_TEMP_XLSB_REG            0xFC

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

/* Filter Macros */
#define BMP280_FILTER_OFF               0x00
#define BMP280_FILTER_COEFF_2           0x01
#define BMP280_FILTER_COEFF_4           0x02
#define BMP280_FILTER_COEFF_8           0x03
#define BMP280_FILTER_COEFF_16          0x04

/* Compensation Parameter storage */
#define BMP280_DIG_T1_LSB               0x88
#define BMP280_DIG_T1_MSB               0x89
#define BMP280_DIG_T2_LSB               0x8A
#define BMP280_DIG_T2_MSB               0x8B

/* Device Struct */
struct bmp280_device {
    uint8_t i2c_adapter;
    uint8_t i2c_addr;
    uint8_t fd;
    uint8_t mode;
    uint8_t temp_os;
    uint8_t pres_os;
};


void bmp280_init(struct bmp280_device *bmp280);

#endif
