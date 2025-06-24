#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


int twim_write(const struct device *dev, uint8_t addr, uint8_t reg, uint8_t *data, uint32_t num_bytes);
int twim_write_noreg(const struct device *dev, uint8_t addr, uint8_t *data, uint32_t num_bytes);
int twim_read(const struct device *dev, uint8_t addr, uint8_t reg, uint8_t *data, uint32_t num_bytes);
int twim_read_noreg(const struct device *dev, uint8_t addr, uint8_t *data, uint32_t num_bytes);
