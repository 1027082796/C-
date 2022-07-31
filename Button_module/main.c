#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "user_multi_button_drive.h"
#include "user_single_io_drive.h"
/************************************************
 ALIENTEK ̽����STM32F407������ ʵ��4
 ����ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
	u8 len;	
	u16 times=0;
	
    HAL_Init();                    	//��ʼ��HAL��    
    Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init(168);               	//��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART
	User_Single_io_Drive_Init();    //��io�豸��ʼ��	
    User_Button_Init();              //��ʼ������
	

    while(1)
    {
			button_ticks(); //run
            Single_io_Drive_ticks();//run
      // printf("run_delay\n");  
			delay_ms(5);   
    }
}

