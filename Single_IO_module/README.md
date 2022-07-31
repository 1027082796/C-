# Single IO Drive

## 简介
Single IO Drive 是一个单io控制设备驱动,控制设备开关.支持自定义频率开关,延迟打开关闭.可使单一控制开关设备集中在一起管理,简洁程序结构.

## 使用方法
1.先申请一个单io结构,在添加io设备的编号

```c
struct Single_io_Drive_t io_drive1; //在 user_single_io_drive.c

enum Single_io_drive_IDs { //单IO设备ID 在 user_single_io_drive.h
    io_drive1_id,
};
```
2.初始化io设备硬件与软件参数

```c
void User_Single_io_Drive_Init(void)
{
    //单IO设备IO硬件初始化，在单IO设备的硬件现实文件
    LED_Init();

    //初始化单io设备结构体
    Single_io_Drive_init(&io_drive1,io_drive1_id,LED0_Open,LED0_Close); // LED0_Open 打开设备函数, LED0_Close 关闭设备函数

    //启动单io设备扫描
    Single_io_Drive_start(&io_drive1);
}
```
3.设置一个5ms间隔的定时器循环调用后台处理函数

```c
//#define SINGLE_IO_TICKS_INTERVAL    5	//ms,  扫描时基, 在 single_io_drive.h
while(1) {
    ...
    if(timer_ticks == 5) {
        timer_ticks = 0;

        Single_io_Drive_ticks();
    }
}
```

## 特性

Single io Drive 使用C语言实现，基于面向对象方式设计思路，每个io设备对象单独用一份数据结构管理：

```c
typedef struct Single_io_Drive_t {
    uint16_t delay_operation_time;   //延迟操作的时间
    uint16_t loop_Open_time;	     //循环开关的打开时间
    uint16_t loop_Close_time;	     //循环开关的关闭时间
    uint16_t loop_count;	     //时间相关的计数标志
    uint8_t  device_id;              //设备编号
    uint8_t  operation_mode : 4;     //操作的模式
    uint8_t  device_state : 1;       //设备当前状态
    uint8_t  time_start_flag : 1;    //时间相关的计数标志
    uint8_t  reserve : 2;            //保留
    void  (*set_open_device)(void);  //打开设备的函数
    void  (*set_close_device)(void); //关闭设备的函数
    struct Single_io_Drive_t* next;  //单链表结构指针
}Single_io_Drive_t;
```
这样每个io设备使用单向链表相连，依次进入 Single_io_Drive_ticks()处理，所以每个io设备的状态彼此独立。


## io设备操作模式

设备操作 | 说明
---|---
OPEN_DEVICE_NO_DELAY | 立即打开设备
CLOSE_DEVICE_NO_DELAY | 立即关闭设备
OPEN_DEVICE_DELAY | 延迟一定时间后打开设备
CLOSE_DEVICE_DELAY | 延迟一定时间后关闭设备
OPEN_AND_CLOSE_LOOP_NO_DELAY | 立即打开设备,并且按设定时间循环开关设备
CLOSE_OPEN_AND_LOOP_NO_DELAY | 立即关闭设备,并且按设定时间循环开关设备
OPEN_AND_CLOSE_LOOP_DELAY | 延迟一定时间后打开设备,并且按设定时间循环开关设备
CLOSE_OPEN_AND_LOOP_DELAY | 延迟一定时间后关闭设备,并且按设定时间循环开关设备

## io设备状态

状态 | 说明
---|---
DEVICE_OPEN_STATE | 当前设备是打开的
DEVICE_CLOSE_STATE | 当前设备是关闭的
DEVICE_INEXISTENT_STATE | 当前设备不存在

## Examples

```c
#include "user_single_io_drive.h"

int main(void)
{
    User_Single_io_Drive_Init();    //单io设备初始化	
	
    while(1)
    {
        Single_io_Drive_ticks();//run
	delay_ms(5);   
    }
}

...



