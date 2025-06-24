#include "lsm6dsv16b.h"

LOG_MODULE_REGISTER(lsm6dsv16b, LOG_LEVEL_NONE);

#if defined(CONFIG_I2C)
bool LSM6DSV16B_init(const struct device *dev, uint8_t device_addr)
{
	const uint8_t INIT_CMD[] = {
		CTRL1,
		CTRL2,
		CTRL6,
		CTRL8,
	};
	const uint8_t INIT_PARAM[] = {
		CTRL1_OP_MODE_XL_LP1 | CTRL1_ODR_XL_240HZ,
		CTRL2_OP_MODE_G_LP | CTRL2_ODR_G_240HZ,
		CTRL6_FS_G_2000DPS,
		CTRL8_FS_XL_16G,
	};
	int err;
	uint8_t twim_buffer[16] = {0};

	// Check IMU sensor
	err = twim_read(dev, device_addr, WHO_AM_I, twim_buffer, 1);
	if (err < 0) {
		LOG_ERR("Device cannot be found.(%d)", err);
		return false;
	} else if (twim_buffer[0] != I_AM_LSM6DSV16B) {
		LOG_ERR("This device is not LSM6DSV16B.");
		return false;
	}

	// Send
	for (uint8_t i = 0; i < sizeof(INIT_CMD); i++) {
		twim_buffer[0] = INIT_PARAM[i];
		err = twim_write(dev, device_addr, INIT_CMD[i], twim_buffer, 1);
		if (err < 0) {
			LOG_ERR("TWIM Error!(%d)", err);
			return false;
		}
	}
	return true;
}
#endif
