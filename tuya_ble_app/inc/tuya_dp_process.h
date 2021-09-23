#ifndef __TUYA_DP_PROCESS_H__
#define __TUYA_DP_PROCESS_H__

union ILLUMINANCE_DATA
{
	unsigned char illuminance_buf[4];
	long illuminance_value;
};

/* tuya data point type */
#define TUYA_DP_TYPE_BOOL		1
#define TUYA_DP_TYPE_VALUE		2
#define TUYA_DP_TYPE_STRING		3
#define TUYA_DP_TYPE_ENUM		4
#define TUYA_DP_TYPE_BITMAP		5

#define ILLUMINANCE_VALUE	101

typedef struct {
	unsigned char dp_id;
	unsigned char dp_type;
	unsigned char dp_data_len;
	unsigned char *dp_data;
}DataPoint;

typedef struct {
	DataPoint illuminance;
}OPT3004;

extern OPT3004 opt3004;
extern union ILLUMINANCE_DATA illuminance_value;

void dp_init(void);
void dp_update(unsigned char dp_id, unsigned char dp_type, unsigned char *dp_date, unsigned short dp_data_len);
void dp_update_all(void);

#endif /* __TUYA_DP_PROCESS_H__ */
