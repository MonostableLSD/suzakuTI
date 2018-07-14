#include "userDefine.h"

void saveEEPROMData()
{
	e2prom_value.offsetRoll=offsetRoll;			//前一个offset为结构体中的，后一个为全局变量，两者不同
	e2prom_value.offsetPitch=offsetPitch;
	e2prom_value.kp_roll=kp_roll;
	e2prom_value.kp_pitch=kp_pitch;
	e2prom_value.kp_yaw=kp_yaw;
	e2prom_value.ki_roll=ki_roll;
	e2prom_value.ki_pitch=ki_pitch;
	e2prom_value.ki_yaw=ki_yaw;
	
	e2prom_value.kp_g_roll=kp_g_roll;
	e2prom_value.kp_g_pitch=kp_g_pitch;
	e2prom_value.kp_g_yaw=kp_g_yaw;
	e2prom_value.ki_g_roll=ki_g_roll;
	e2prom_value.ki_g_pitch=ki_g_pitch;
	e2prom_value.ki_g_yaw=ki_g_yaw;
	e2prom_value.kd_g_roll=kd_g_roll;
	e2prom_value.kd_g_pitch=kd_g_pitch;
	e2prom_value.kd_g_yaw=kd_g_yaw;	
	e2prom_value.kp_h=kp_h;
	e2prom_value.ki_h=ki_h;
	e2prom_value.kd_h=kd_h;
	
	e2prom_value.kp_x=kp_x;
	e2prom_value.kp_y=kp_y;
	e2prom_value.ki_x=ki_x;
	e2prom_value.ki_y=ki_y;
	e2prom_value.kd_x=kd_x;
	e2prom_value.kd_y=kd_y;

	ROM_EEPROMProgram((uint32_t *)&e2prom_value, 0x0000, sizeof(e2prom_value));
}
