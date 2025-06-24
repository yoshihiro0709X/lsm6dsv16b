#include <dk_buttons_and_leds.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include "twim.h"
#include "lsm6dsv16b.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// Semaphore
K_SEM_DEFINE(sem_sensor, 0, 1);

// Device
const struct device *dev_twim0 = DEVICE_DT_GET(DT_NODELABEL(i2c0));


static void read_sensor_work_thread(void)
{
	int err;
	uint8_t twim_buffer[16];
	int16_t accel[3], gyro[3];

	while (true) {
		k_sem_take(&sem_sensor, K_FOREVER);

		// Read IMU sensor(s)
		err = twim_read(dev_twim0, LSM6DSV16B_ADDR1, OUTX_L_G, twim_buffer, 12);
		if (!err) {
			accel[0] = (int16_t)(twim_buffer[11] << 8) | twim_buffer[10];
			accel[1] = (int16_t)(twim_buffer[9] << 8) | twim_buffer[8];
			accel[2] = (int16_t)(twim_buffer[7] << 8) | twim_buffer[6];
			gyro[0] = (int16_t)(twim_buffer[1] << 8) | twim_buffer[0];
			gyro[1] = (int16_t)(twim_buffer[3] << 8) | twim_buffer[2];
			gyro[2] = (int16_t)(twim_buffer[5] << 8) | twim_buffer[4];

			// Display value(s) for debug
			LOG_INF("LSM6DSV16B ACC XYZ = %+6d, %+6d, %+6d", accel[0], accel[1], accel[2]);
			LOG_INF("LSM6DSV16B GYR XYZ = %+6d, %+6d, %+6d", gyro[0], gyro[1], gyro[2]);
		} else {
			LOG_ERR("Failed to read from IMU sensor.(%d)", err);
		}
	}
}
K_THREAD_DEFINE(read_sensor, 1024, read_sensor_work_thread, NULL, NULL, NULL, K_PRIO_PREEMPT(0), 0, 0);

/* ----- Timer Function Start ----- */
static void timer_stop_handler(struct k_timer *timer)
{
	LOG_INF("Timer stopped.");
}

static void timer_handler(struct k_timer *timer)
{
	// Resume thread
	LOG_DBG("Read sensor(s).");
	k_sem_give(&sem_sensor);
}
K_TIMER_DEFINE(timer, timer_handler, timer_stop_handler);
/* ----- Timer Function End ----- */

int main(void)
{
	int err;

	// Initialize LEDs
	err = dk_leds_init();
	if (err) {
		LOG_ERR("Cannot init LEDs.(%d)", err);
		return -1;
	}

	// Initialize
	if (LSM6DSV16B_init(dev_twim0, LSM6DSV16B_ADDR1)) {
		LOG_INF("IMU sensor has been initialized.");
	} else {
		LOG_ERR("Failed to initialize IMU sensor.(%d)", err);
		return -1;
	}

	// Timer
	k_timer_start(&timer, K_NO_WAIT, K_MSEC(1000));

	// Main
	while (true) {
		dk_set_led_on(0);
		k_msleep(500);
		dk_set_led_off(0);
		k_msleep(500);
		dk_set_led_on(1);
		k_msleep(500);
		dk_set_led_off(1);
		k_msleep(500);
	}
}
