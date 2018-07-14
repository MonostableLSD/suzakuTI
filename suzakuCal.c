#include "userDefine.h"

void ATT_TEMP()
{	
	static u8 cnt=0;
	static float sum_yaw=0;
	if(flag.set_yaw==0) 		//获取初始yaw角
	{	
		cnt++;
		sum_yaw+=yaw;
		if(cnt==5)
		{
			tarYaw=sum_yaw/5;
			flag.set_yaw=1;
		}
	}
	roll_temp=roll-offsetRoll;
	pitch_temp=pitch-offsetPitch;
	yaw_temp=yaw-tarYaw;
	
}
void motoCal()		//何时启用某一控制 取决于是否有值加在了moto上 在tick里改变flag以此控制控制的生命周期
{
	static u8 flightMode=0;
	if(flag.ct_att_hold&&!flag.ct_alt_hold&&!flag.ct_loc_hold)	flightMode=1;				//只有姿态控制
	
	else if(flag.ct_att_hold && flag.ct_alt_hold&&!flag.ct_loc_hold) flightMode=2;	//姿态+高度
	
	else if(flag.ct_att_hold&&flag.ct_alt_hold&&flag.ct_loc_hold)	flightMode=3;			//姿态+高度+位置
	
	else if(!flag.ct_att_hold&&flag.ct_alt_hold&&!flag.ct_loc_hold)flightMode=4; //只有高度控制 目标高度30cm
	else if(!flag.ct_att_hold&&!flag.ct_alt_hold&&flag.ct_loc_hold)	flightMode=5; //只有位置控制
	else if(!flag.ct_att_hold&&!flag.ct_alt_hold&&!flag.ct_loc_hold) flightMode=6; //不加入任何控制 测试用
	switch (flightMode)
	{
		case 1:		//只有姿态控制 41
					moto1=(u32)(MIN_PWMPULSE+att_cal1+throttle);
					moto2=(u32)(MIN_PWMPULSE+att_cal2+throttle);	
					moto3=(u32)(MIN_PWMPULSE+att_cal3+throttle);	
					moto4=(u32)(MIN_PWMPULSE+att_cal4+throttle);	
		
		break;
		
		case 2:	//姿态+高度 44
					moto1=(u32)(MIN_PWMPULSE+att_cal1+alt_cal1+throttle);
					moto2=(u32)(MIN_PWMPULSE+att_cal2+alt_cal2+throttle);
					moto3=(u32)(MIN_PWMPULSE+att_cal3+alt_cal3+throttle);
					moto4=(u32)(MIN_PWMPULSE+att_cal4+alt_cal4+throttle);	
		break;
		
		case 3://姿态+高度+位置 45
					moto1=(u32)(MIN_PWMPULSE+att_cal1+alt_cal1+loc_cal1+throttle);
					moto2=(u32)(MIN_PWMPULSE+att_cal2+alt_cal2+loc_cal2+throttle);
					moto3=(u32)(MIN_PWMPULSE+att_cal3+alt_cal3+loc_cal3+throttle);
					moto4=(u32)(MIN_PWMPULSE+att_cal4+alt_cal4+loc_cal4+throttle);	
		break;
		
		case 4://只有高度控制 目标高度30cm  42
					moto1=(u32)(MIN_PWMPULSE+alt_cal1+throttle);
					moto2=(u32)(MIN_PWMPULSE+alt_cal2+throttle);
					moto3=(u32)(MIN_PWMPULSE+alt_cal3+throttle);
					moto4=(u32)(MIN_PWMPULSE+alt_cal4+throttle);	
			break;
		
		case 5://只有位置控制 43
					moto1=(u32)(MIN_PWMPULSE+loc_cal1+throttle);
					moto2=(u32)(MIN_PWMPULSE+loc_cal2+throttle);
					moto3=(u32)(MIN_PWMPULSE+loc_cal3+throttle);
					moto4=(u32)(MIN_PWMPULSE+loc_cal4+throttle);	
			break;
		
		case 6:	//不加入任何控制 测试用
					moto1=(u32)(MIN_PWMPULSE+throttle);	
					moto2=(u32)(MIN_PWMPULSE+throttle);	
					moto3=(u32)(MIN_PWMPULSE+throttle);	
					moto4=(u32)(MIN_PWMPULSE+throttle);	
		break;
			  
	}

}
