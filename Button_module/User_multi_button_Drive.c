#include "user_multi_button_drive.h"



struct Button btn1; //按键结构体
struct Button btn2; //按键结构体

/**
  * @brief  用于读取按键状态,所有按键读取函数丢这里面.
  * @param  button_id: 按键编号.
  * @retval None
  */
uint8_t read_all_button_GPIO(uint8_t button_id) 
{
	switch(button_id)
	{
		case btn1_id:
			return KEY0;  //与按键编号对应的读取按键状态函数

		case btn2_id:
			return KEY1;  //与按键编号对应的读取按键状态函数

		default:
			return 0;
	}
}

/**
  * @brief  对应按键的回调事件处理函数.
  * @param  button: 需要注册事件的按键结构体.
  * @retval None
  */
void button1_callback(void *button)
{
    uint32_t btn_event_val; 
    
    btn_event_val = get_button_event((struct Button *)button); 
    //printf("btn_event_val=%d\n",btn_event_val);
    switch(btn_event_val)
    {
	    case PRESS_DOWN:
	        printf("---> key1 press down! <---\r\n"); 
	    	break; 
	
	    case PRESS_UP: 
	        printf("***> key1 press up! <***\r\n");
	    	break; 
	
	    case PRESS_REPEAT: 
	        printf("---> key1 press repeat! %d <---\r\n",((struct Button *)button)->repeat);
	    	break; 
	
	    case SINGLE_CLICK: 
	        printf("---> key1 single click! <---\r\n");
			Set_Single_io_Drive_State(io_drive1_id,OPEN_AND_CLOSE_LOOP_DELAY,2000,1000,2000);
	    	break; 
	
	    case DOUBLE_CLICK: 
	        printf("***> key1 double click! <***\r\n");
			Set_Single_io_Drive_State(io_drive1_id,CLOSE_DEVICE_NO_DELAY,0,0,0);
	    	break; 
	
	    case LONG_PRESS_START: 
	        printf("---> key1 long press start! <---\r\n");
	   		break; 
	
	    case LONG_PRESS_HOLD: 
	        printf("***> key1 long press hold! <***\r\n");
	    	break; 
	}
}


/**
  * @brief  对应按键的回调事件处理函数.
  * @param  button: 需要注册事件的按键结构体.
  * @retval None
  */
void button2_callback(void *button)
{
    uint32_t btn_event_val; 
    
    btn_event_val = get_button_event((struct Button *)button); 
    //printf("btn_event_val=%d\n",btn_event_val);
    switch(btn_event_val)
    {
	    case SINGLE_CLICK: 
	        printf("---> key2 single click! <---\r\n");
			Set_Single_io_Drive_State(io_drive2_id,OPEN_AND_CLOSE_LOOP_DELAY,2000,1000,2000);
	    	break; 
	
	    case DOUBLE_CLICK: 
	        printf("***> key2 double click! <***\r\n");
			Set_Single_io_Drive_State(io_drive2_id,CLOSE_DEVICE_NO_DELAY,0,0,0);
	    	break; 
	}
}

/**
  * @brief  按键硬件io与软件参数初始化.
  * @retval None
  */
void User_Button_Init(void)
{
    //按键IO硬件初始化，在按键的硬件现实文件
    KEY_Init();

    //初始化按键结构体
    button_init(&btn1, read_all_button_GPIO, LOW_ACTIVE_LEVEL, btn1_id);
	button_init(&btn2, read_all_button_GPIO, LOW_ACTIVE_LEVEL, btn2_id);

	//注册按钮事件回调函数
	button_attach(&btn1, PRESS_DOWN,       button1_callback);
	button_attach(&btn1, PRESS_UP,         button1_callback);
	button_attach(&btn1, PRESS_REPEAT,     button1_callback);
	button_attach(&btn1, SINGLE_CLICK,     button1_callback);
	button_attach(&btn1, DOUBLE_CLICK,     button1_callback);
	button_attach(&btn1, LONG_PRESS_START, button1_callback);
	button_attach(&btn1, LONG_PRESS_HOLD,  button1_callback);

	button_attach(&btn2, SINGLE_CLICK,     button2_callback);
	button_attach(&btn2, DOUBLE_CLICK,     button2_callback);
	
	//启动按键扫描
	button_start(&btn1);
	button_start(&btn2);
}








