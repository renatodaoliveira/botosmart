#include "vl53l0x.h"
#include "pico/stdlib.h"
#include <string.h>

static inline bool write_reg(i2c_inst_t *i2c, uint8_t reg, uint8_t val) {
    uint8_t buf[] = {reg, val};
    return i2c_write_blocking(i2c, VL53L0X_I2C_ADDR, buf, 2, false) == 2;
}

static inline bool read_reg16(i2c_inst_t *i2c, uint8_t reg, uint16_t *val) {
    uint8_t buf[2];
    if (i2c_write_blocking(i2c, VL53L0X_I2C_ADDR, &reg, 1, true) != 1) return false;
    if (i2c_read_blocking(i2c, VL53L0X_I2C_ADDR, buf, 2, false) != 2) return false;
    *val = (buf[0] << 8) | buf[1];
    return true;
}

bool vl53l0x_init(i2c_inst_t *i2c) {
    sleep_ms(10);
    return write_reg(i2c, 0x88, 0x00);
}

bool vl53l0x_start_ranging(i2c_inst_t *i2c) {
    return write_reg(i2c, 0x00, 0x01);
}

bool vl53l0x_read_distance(i2c_inst_t *i2c, uint16_t *distance) {
    uint8_t status;
    int tries = 100;
    while (tries--) {
        i2c_write_blocking(i2c, VL53L0X_I2C_ADDR, (uint8_t[]){0x14}, 1, true);
        i2c_read_blocking(i2c, VL53L0X_I2C_ADDR, &status, 1, false);
        if (status & 0x01) break;
        sleep_ms(10);
    }

    return read_reg16(i2c, 0x1E, distance);
}