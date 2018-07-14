2017.8.13  黄铮
2018.3.18  Aemaeth2501
//			SysCtlDelay(10 * (SysCtlClockGet() / 300));
uart1数据处理优化
FPU功能
匿名上位机功能待添加
NVIC中断优先级待优化
数据类型待优化

基于TM4C123GH6PM的四旋翼飞行器控制系统

APP: 应用层

BSP: 所有硬件相关的代码封装在bsp中，APP中只调用接口  

Lib: 调用tiva库    include: driverlib
							utils 
							IQmath..
							
C/C++->DEFINE:  rvmdk PART_TM4C123GH6PM 
				TARGET_IS_TM4C123_RB1	//为了使用ROM_FUNCTION();

Target: >Use MicroLib (fputc()printf())


UART0 (PA0 RX PA1 TX) for HC05/Simulation debugger to detect data,send command or adjust parameters;

UART1 (PB0 RX PB1 TX) for JY901 to get attitude;

UART5 () for ANO_LightFlow;


//UART3 (PC6 RX PC7 TX) for US100
//								  to get height;
//IIC1  (PA6 SCL PA7 SDA) for GY530 
//
//UART2 (PD6 RX PD7 TX)for navigation device;



[PF0 PD7 PC0 注意解锁]http://bbs.21ic.com/forum.php?mod=viewthread&tid=1571214


PWM0 				PB6
PWM1				PB7
PWM2				PB4
PWM3				PB5

PWMperiod 4ms frequency 250hz 20000

相关阅读: 

BSP:	http://blog.csdn.net/qsj8362234/article/details/6325761							