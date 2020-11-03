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

#endif
