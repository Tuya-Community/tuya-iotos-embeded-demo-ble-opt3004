#include "tuya_ble_api.h"

#include "tuya_dp_process.h"

union ILLUMINANCE_DATA illuminance_value;

OPT3004 opt3004;

void dp_init(void)
{
	opt3004.illuminance.dp_id = ILLUMINANCE_VALUE;
	opt3004.illuminance.dp_type = TUYA_DP_TYPE_VALUE;
	opt3004.illuminance.dp_data_len = 4;
	opt3004.illuminance.dp_data = illuminance_value.illuminance_buf;
}

void dp_update(unsigned char dp_id, unsigned char dp_type, unsigned char *dp_date, unsigned short dp_data_len)
{
	uint16_t i = 0;
	unsigned char dp_data_array[16];

	dp_data_array[0] = dp_id;
	dp_data_array[1] = dp_type;
	dp_data_array[2] = dp_data_len;
	for (i = 0; i < dp_data_array[2]; i++) { //将联合体中数据颠倒
		dp_data_array[i + 3] = *(dp_date + (3-i));
	}
	tuya_ble_dp_data_report(dp_data_array, (dp_data_array[2] + 3));
}

void dp_update_all(void)
{
	dp_update(opt3004.illuminance.dp_id,
			opt3004.illuminance.dp_type,
			opt3004.illuminance.dp_data,
			opt3004.illuminance.dp_data_len);
}
