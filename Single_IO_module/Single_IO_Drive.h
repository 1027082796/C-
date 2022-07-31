#ifndef _SINGLE_IO_DRIVE_H_
#define _SINGLE_IO_DRIVE_H_

#include "stdint.h"
#include "string.h"


/***************According to your need to modify the constants.*****************************************************/
#define SINGLE_IO_TICKS_INTERVAL    5	//ms,
/********************************************************************************************************************/

//device state
typedef enum { 
	DEVICE_OPEN_STATE  = 0,  //The device is on
	DEVICE_CLOSE_STATE,      //The device is off
    DEVICE_INEXISTENT_STATE  //The device is inexistent
}Device_state_e;

//device operation mode state
typedef enum { 
    //Operational modes available
	OPEN_DEVICE_NO_DELAY  = 0,      //Turn the device on immediately.
	CLOSE_DEVICE_NO_DELAY,          //Shut down the device immediately.
    OPEN_DEVICE_DELAY,              //Delayed opening device
	CLOSE_DEVICE_DELAY,             //Delayed shutdown of equipment
    OPEN_AND_CLOSE_LOOP_NO_DELAY,   //Loop on and off,There is no delay for the first operation
    CLOSE_OPEN_AND_LOOP_NO_DELAY,   //Loop off and on,There is no delay for the first operation
    OPEN_AND_CLOSE_LOOP_DELAY,      //Loop on and off,There is a delay in the first operation
    CLOSE_OPEN_AND_LOOP_DELAY,      //Loop off and on,There is a delay in the first operation
    //The operation mode cannot be used by the user
    NOW_COUNT_START,                //Count start
    NOW_COUNT_STOP,                 //Count to stop
    NOW_LOOP_MODE,                  //The device is circulating on and off
    NONE_OPEN_CLOSE                 //No operations
}Device_Operation_State_e;

typedef struct Single_io_Drive_t {
	uint16_t delay_operation_time;	 //Delayed operation time
    uint16_t loop_Open_time;	     //Cycle opening time
    uint16_t loop_Close_time;	     //Cycle closing time
    uint16_t loop_count;	         //Loop mode, count flags
    uint8_t  device_id;              //Device ID index
	uint8_t  operation_mode : 4;     //Current Operating Mode
    uint8_t  device_state : 1;       //Current Device Status
    uint8_t  time_start_flag : 1;    //The current timing mode starts
    uint8_t  reserve : 2;            //no use
	void  (*set_open_device)(void);  //open device
    void  (*set_close_device)(void); //close device
	struct Single_io_Drive_t* next;  //Next single IO device address
}Single_io_Drive_t;



#ifdef __cplusplus
extern "C" {
#endif

void Single_io_Drive_init(struct Single_io_Drive_t* handle, uint8_t device_id, void(*open_device)(void), void(*close_device)(void));
int Single_io_Drive_start(struct Single_io_Drive_t* handle);
void Single_io_Drive_stop(struct Single_io_Drive_t* handle);
Device_state_e Get_Single_io_Drive_State(uint8_t device_id);
int Set_Single_io_Drive_State(uint8_t device_id, Device_Operation_State_e mode, uint16_t delay_time, uint16_t loop_open_time, uint16_t loop_close_time );
void Single_io_Drive_handler(struct Single_io_Drive_t* handle);
void Single_io_Drive_ticks(void);

#ifdef __cplusplus
}
#endif

#endif



