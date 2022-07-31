#ifndef _USER_SINGLE_IO_DRIVE_H_
#define _USER_SINGLE_IO_DRIVE_H_

#include "single_io_drive.h"
#include "led.h" //按键的硬件现实文件

enum Single_io_drive_IDs { //单IO设备ID
	io_drive1_id,
    io_drive2_id,
};



#ifdef __cplusplus
extern "C" {
#endif

void User_Single_io_Drive_Init(void);

#ifdef __cplusplus
}
#endif


#endif
