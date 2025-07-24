#ifndef VL53L0X_H
#define VL53L0X_H

#include "hardware/i2c.h"
#include <stdbool.h>

#define VL53L0X_I2C_ADDR 0x29

bool vl53l0x_init(i2c_inst_t *i2c);
bool vl53l0x_start_ranging(i2c_inst_t *i2c);
bool vl53l0x_read_distance(i2c_inst_t *i2c, uint16_t *distance);

#endif