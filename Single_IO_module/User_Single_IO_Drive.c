#include "user_single_io_drive.h"


struct Single_io_Drive_t io_drive1; //单设备结构体
struct Single_io_Drive_t io_drive2; //单设备结构体


/**
  * @brief  单设备硬件io与软件参数初始化.
  * @retval None
  */
void User_Single_io_Drive_Init(void)
{
    //单IO设备IO硬件初始化，在单IO设备的硬件现实文件
    LED_Init();

    //初始化单io设备结构体
    Single_io_Drive_init(&io_drive1,io_drive1_id,LED0_Open,LED0_Close);
    Single_io_Drive_init(&io_drive2,io_drive2_id,LED1_Open,LED1_Close);

	//启动单io设备扫描
	Single_io_Drive_start(&io_drive1);
    Single_io_Drive_start(&io_drive2);
}





