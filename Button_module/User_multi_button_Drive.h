#ifndef _USER_MULTI_BUTTON_H_
#define _USER_MULTI_BUTTON_H_

#include "multi_button_drive.h"
//添加需要函数的头文件
#include "key.h" //按键的硬件现实文件
#include "user_single_io_drive.h"

enum Button_IDs { //按键ID
	btn1_id,
	btn2_id,
};



#ifdef __cplusplus
extern "C" {
#endif

void User_Button_Init(void);


#ifdef __cplusplus
}
#endif


#endif
