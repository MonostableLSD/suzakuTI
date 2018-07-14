#include "userDefine.h"
//void ATT_Ctrl()
//{
//	flag.ct_att_hold=1;
//	flag.ct_alt_hold=0;
//	flag.ct_loc_hold=0;
//}
//void ATL_Ctrl()
//{	
//	flag.ct_att_hold=0;
//	flag.ct_alt_hold=1;
//	flag.ct_loc_hold=0;
//}
//void LOC_Ctrl()
//{
//	flag.ct_att_hold=0;
//	flag.ct_alt_hold=0;
//	flag.ct_loc_hold=1;
//}

//通过控制FLAG.ATT//ALT(TAR_HEIGHT)//LOC_HOLD
void Auto_take_off()	//纯姿态自动起飞 tarHeight increase
{
//	if(OF_ALT2<7)
//	{

//		//ATT_Ctrl();

//	
//	}
}				

//判断自动降落flag 再判断高度 如果小于20 那么flag stop 打开 停在手上
void Auto_land()			//纯姿态自动降落
{
		static u8 i=0;
		if((tarHeight-2)<OF_ALT2)//在这里做一个标志位判断 如果连续大于tar_height-5 6次 那么自动降落flag打开 
						{	i++;
							if(i>20)
								{
								//	printf("flag.landing=1");
									flag.landing=1;
									i=0;
								}
						}
					if(flag.landing==1&&OF_ALT2<20)
					{
//							flag.all_stop=1;
			redLed();//Red LED means "all stop"
			motoRefresh(MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE);
			ROM_IntMasterDisable();
					}
					
					//ATT_Ctrl();
	//if(OF_ALT2>2) TAR_HEIGHT--; 可能会滞后
	//else if(OF_ALT2<=2) 
	//			
	//			motoRefresh(MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE);
		
}
void Auto_hover()			//悬停 tarHeight=30
{
		//ATT_Ctrl();
		//Height_Ctrl();
		//Loc_Ctrl();
}

void safetyCheck()		//安全检查 紧急停止
{ static u8 i=0;
	if(fabs(roll)>30||fabs(pitch)>30||OF_ALT2>120||flag.all_stop==1)
	{
//		printf("sc");
		i++;
		if(i==3)
		{
			i=0;
		//	printf("stop");
			redLed();//Red LED means "all stop"
			motoRefresh(MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE);
			ROM_IntMasterDisable();
		}
	}
	
}
