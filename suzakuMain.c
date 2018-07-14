#include "userDefine.h"

/*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
*****************************************************************************/
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif



int main(void)
{
	flag.start_ok=All_init();
	motoRefresh(MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE);
	
	while(1)
	{	
//					printf("moto1:%d   height:%d  roll:%.2f   pitch:%.2f   yaw:%.2f.\n",moto1,OF_ALT2,roll,pitch,yaw);
//					printf("OF_DX2:%d   OF_DY2:%d  OF_DX2FIX:%d   OF_DY2FIX:%d\n",OF_DX2,OF_DY2,OF_DX2FIX,OF_DY2FIX);	
//					printf("%d,%.2f,%.2f,%.2f,%.2f\n",throttle,loc_cal1,loc_cal2,loc_cal3,loc_cal4);
//			if(ROM_GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_2)==0)
//			{			
//					motoRefresh(MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE,MIN_PWMPULSE);
////				flag.ct_att_hold=1;
////				flag.ct_alt_hold=0;
////				flag.ct_loc_hold=0;
//			}
//			
//			if(ROM_GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_3)==0)
//			{
//					motoRefresh(MAX_PWMPULSE,MAX_PWMPULSE,MAX_PWMPULSE,MAX_PWMPULSE);
////				flag.ct_att_hold=0;
////				flag.ct_alt_hold=1;
////				flag.ct_loc_hold=0;
//			}
//			if(ROM_GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_4)==0)
//			{
//				motoRefresh(6000,6000,6000,6000);
////				flag.ct_att_hold=0;
////				flag.ct_alt_hold=0;
////				flag.ct_loc_hold=1;
//			}
	}
}
