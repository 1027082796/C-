#ifndef _MULTI_BUTTON_DRIVE_H_
#define _MULTI_BUTTON_DRIVE_H_

#include "stdint.h"
#include "string.h"

/***************According to your need to modify the constants.*****************************************************/
#define BUTTON_TICKS_INTERVAL    5	//ms,
#define DEBOUNCE_TICKS    3	//MAX 8, key shaking elimination, Based on the time base (BUTTON_TICKS_INTERVAL) 
#define SHORT_TICKS       (300 /BUTTON_TICKS_INTERVAL)  //Click Trigger time
#define LONG_TICKS        (1000 /BUTTON_TICKS_INTERVAL) //Double click trigger time
/********************************************************************************************************************/

typedef void (*BtnCallback)(void*);

//Key effective level
typedef enum { 
	LOW_ACTIVE_LEVEL  = 0, 
	HIGH_ACTIVE_LEVEL = 1,
}Btn_ActiveLevel_e;

//key events
typedef enum {
	PRESS_DOWN = 0,   //press
	PRESS_UP,	      //loosen
	PRESS_REPEAT,     //Repeat to press
	SINGLE_CLICK,	  //Click on the button
	DOUBLE_CLICK,	  //Double-click the button
	LONG_PRESS_START, //Key Long press to start
	LONG_PRESS_HOLD,  //Key Long press to hold
	number_of_event,  //Maximum number of key events
	NONE_PRESS        //No keystroke event
}PressEvent;

typedef struct Button {
	uint16_t ticks;	
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3;
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	uint8_t  button_id;
	uint8_t  (*hal_button_Level)(uint8_t button_id_);
	BtnCallback  cb[number_of_event];
	struct Button* next;
}Button;

#ifdef __cplusplus
extern "C" {
#endif

void button_init(struct Button* handle, uint8_t(*pin_level)(uint8_t button_id), uint8_t active_level, uint8_t button_id);
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
void button_ticks(void);

#ifdef __cplusplus
}
#endif

#endif
