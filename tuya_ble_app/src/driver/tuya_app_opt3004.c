#include "app_config.h"
#include "drivers.h"
#include "drivers/8258/i2c.h"

#include "tuya_ble_log.h"

#include "tuya_app_opt3004.h"
#include "tuya_dp_process.h"


#define I2C_CLK_SPEED 200000

short lsb_size_tab[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

unsigned char opt3004_init(void)
{
    unsigned char device_id_data_buf[2] = {0};
    unsigned char manufacturer_id_data_buf[2] = {0};

    unsigned char write_data[2] = {0xcc, 0x10};

    unsigned long delay_time = 0;

    i2c_gpio_set(I2C_GPIO_GROUP_C0C1);

    i2c_master_init(OPT3004_I2C_ADDR_W, (unsigned char)(CLOCK_SYS_CLOCK_HZ / (4 * I2C_CLK_SPEED)));
    i2c_write_series(OPT3004_CONFIG_REGISTER_ADDR, 1, write_data, 2);

    i2c_master_init(OPT3004_I2C_ADDR_R, (unsigned char)(CLOCK_SYS_CLOCK_HZ / (4 * I2C_CLK_SPEED)));

    i2c_read_series(OPT3004_DEVICE_ID_REGISTER_ADDR, 1, device_id_data_buf, 2);
    delay_time = clock_time();
    while (!clock_time_exceed(delay_time, 100)); //100us delay
    i2c_read_series(OPT3004_MANUFACTURER_ID_REGISTER_ADDR, 1, manufacturer_id_data_buf, 2);
    if ((((device_id_data_buf[0]<<8) + device_id_data_buf[1]) != DEVICE_ID) || \
		(((manufacturer_id_data_buf[0]<<8) + manufacturer_id_data_buf[1]) != MANUFACTURER_ID)) {
        return 0;
    }

    return 1;
}

short get_opt3004_value(void)
{
	short ret_value = -1;
	int result_value = 0;
	short result_data_e = 0, result_data_r = 0;
	unsigned char opt3004_cfg_data[2] = {0};
	unsigned char opt3004_result_data[2] = {0};

	i2c_master_init(OPT3004_I2C_ADDR_R, (unsigned char)(CLOCK_SYS_CLOCK_HZ / (4 * I2C_CLK_SPEED)));

	i2c_read_series(OPT3004_CONFIG_REGISTER_ADDR, 1, opt3004_cfg_data, 2);
	if (opt3004_cfg_data[1]&0x80) {
		i2c_read_series(OPT3004_RESULT_REGISTER_ADDR, 1, opt3004_result_data, 2);
		result_value = (opt3004_result_data[0]<<8) + opt3004_result_data[1];

		result_data_e = ((result_value & 0xF000) >> 12);
		result_data_r = (result_value & 0x0FFF);
		ret_value = lsb_size_tab[result_data_e] * result_data_r / 100;
		//TUYA_APP_LOG_DEBUG("ret_value:%d", ret_value);
	}
	return ret_value;
}

//unsigned long get_illumination_time = 0;
unsigned long update_illumination_time = 0;

void update_current_illumination(void)
{
	illuminance_value.illuminance_value = get_opt3004_value();
	dp_update(opt3004.illuminance.dp_id,
			opt3004.illuminance.dp_type,
			opt3004.illuminance.dp_data,
			opt3004.illuminance.dp_data_len);
}

void update_current_illumination_task(void)
{
	if (clock_time_exceed(update_illumination_time, 500 * 1000)) {
		update_illumination_time = clock_time();
		update_current_illumination();
	}
}
