#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "user_multi_button_drive.h"
#include "user_single_io_drive.h"
/************************************************
 ALIENTEK 探索者STM32F407开发板 实验4
 串口实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
	u8 len;	
	u16 times=0;
	
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);              //初始化USART
	User_Single_io_Drive_Init();    //单io设备初始化	
    User_Button_Init();              //初始化按键
	

    while(1)
    {
			button_ticks(); //run
            Single_io_Drive_ticks();//run
      // printf("run_delay\n");  
			delay_ms(5);   
    }
}

