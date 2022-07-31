# MultiButton

## 简介
MultiButton 是一个小巧简单易用的事件驱动型按键驱动模块，可无限量扩展按键，按键事件的回调异步处理方式可以简化你的程序结构，去除冗余的按键处理硬编码，让你的按键业务逻辑更清晰。

## 使用方法
1.先申请一个按键结构,在添加按键编号

```c
struct Button btn1; //在user_multi_button_drive.c

enum Button_IDs { //按键ID  在user_multi_button_drive.h
	btn1_id,
};
```
2.添加按键读取函数,所有按键读取都添加到这里

```c
uint8_t read_all_button_GPIO(uint8_t button_id) //在user_multi_button_drive.c
{
	switch(button_id)
	{
		case btn1_id:
			return KEY0;  //KEY0 是按键编号对应的读取按键状态函数
			
		default:
			return 0;
	}
}
```
3.添加按键事件处理,只写需要处理的事件

```c
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
...

4.初始化按键参数

```c
void User_Button_Init(void)
{
    //按键IO硬件初始化，在按键的硬件现实文件
    KEY_Init();

    //初始化按键结构体
    button_init(&btn1, read_all_button_GPIO, LOW_ACTIVE_LEVEL, btn1_id);

    //注册按钮事件回调函数
    button_attach(&btn1, PRESS_DOWN,       button1_callback);
    button_attach(&btn1, PRESS_UP,         button1_callback);
    button_attach(&btn1, PRESS_REPEAT,     button1_callback);
    button_attach(&btn1, SINGLE_CLICK,     button1_callback);
    button_attach(&btn1, DOUBLE_CLICK,     button1_callback);
    button_attach(&btn1, LONG_PRESS_START, button1_callback);
    button_attach(&btn1, LONG_PRESS_HOLD,  button1_callback);
	
    //启动按键扫描
    button_start(&btn1);
}
```
5.设置一个5ms间隔的定时器循环调用后台处理函数

```c
while(1) {
    ...
    if(timer_ticks == 5) {
        timer_ticks = 0;

        button_ticks();
    }
}
```

## 特性

MultiButton 使用C语言实现，基于面向对象方式设计思路，每个按键对象单独用一份数据结构管理：

```c
struct Button {
	uint16_t ticks;
	uint8_t  repeat: 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3;
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	uint8_t  button_id;
	uint8_t  (*hal_button_Level)(uint8_t  button_id_);
	BtnCallback  cb[number_of_event];
	struct Button* next;
};
```
这样每个按键使用单向链表相连，依次进入 button_handler(struct Button* handle) 状态机处理，所以每个按键的状态彼此独立。


## 按键事件

事件 | 说明
---|---
PRESS_DOWN | 按键按下，每次按下都触发
PRESS_UP | 按键弹起，每次松开都触发
PRESS_REPEAT | 重复按下触发，变量repeat计数连击次数
SINGLE_CLICK | 单击按键事件
DOUBLE_CLICK | 双击按键事件
LONG_PRESS_START | 达到长按时间阈值时触发一次
LONG_PRESS_HOLD | 长按期间一直触发


## Examples

```c
#include "user_multi_button_drive.h"

int main(void)
{
    User_Button_Init();              //初始化按键
	

    while(1)
    {
	button_ticks(); //run
	delay_ms(5);   
    }
}
```


