#include "twim.h"

LOG_MODULE_REGISTER(twim, LOG_LEVEL_INF);


#if defined(CONFIG_I2C)
int twim_write(const struct device *dev, uint8_t addr, uint8_t reg, uint8_t *data, uint32_t num_bytes)
{
	int err;
	struct i2c_msg msgs[2];

	/* Send the address to write to */
	msgs[0].buf = &reg;
	msgs[0].len = 1U;
	msgs[0].flags = I2C_MSG_WRITE;

	/* Data to be written, and STOP after this. */
	msgs[1].buf = data;
	msgs[1].len = num_bytes;
	msgs[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	err = i2c_transfer(dev, &msgs[0], 2, addr);
	return err;
}

int twim_write_noreg(const struct device *dev, uint8_t addr, uint8_t *data, uint32_t num_bytes)
{
	int err;
	struct i2c_msg msgs;

	/* Data to be written, and STOP after this. */
	msgs.buf = data;
	msgs.len = num_bytes;
	msgs.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	err = i2c_transfer(dev, &msgs, 1, addr);
	return err;
}

int twim_read(const struct device *dev, uint8_t addr, uint8_t reg, uint8_t *data, uint32_t num_bytes)
{
	int err;
	struct i2c_msg msgs[2];

	/* Send the address to read from */
	msgs[0].buf = &reg;
	msgs[0].len = 1U;
	msgs[0].flags = I2C_MSG_WRITE;

	/* Read from device. STOP after this. */
	msgs[1].buf = data;
	msgs[1].len = num_bytes;
	msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

	err = i2c_transfer(dev, &msgs[0], 2, addr);
	return err;
}

int twim_read_noreg(const struct device *dev, uint8_t addr, uint8_t *data, uint32_t num_bytes)
{
	int err;
	struct i2c_msg msgs;

	/* Read from device. STOP after this. */
	msgs.buf = data;
	msgs.len = num_bytes;
	msgs.flags = I2C_MSG_READ | I2C_MSG_STOP;

	err = i2c_transfer(dev, &msgs, 1, addr);
	return err;
}
#endif
