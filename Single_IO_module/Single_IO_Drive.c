#include "single_io_drive.h"


//single io drive handle list head.
static struct Single_io_Drive_t* head_handle = NULL;

/**
  * @brief  Initializes the single io drive struct handle.
  * @param  handle: the drive handle strcut.
  * @param  device_id: the button id. 
  * @param  open_device: Open device function
  * @param  close_device: Close device function
  * @retval None
  */
void Single_io_Drive_init(struct Single_io_Drive_t* handle, uint8_t device_id, void(*open_device)(void), void(*close_device)(void))
{
	memset(handle, 0, sizeof(struct Single_io_Drive_t));
	handle->device_id=device_id;
  handle->device_state=DEVICE_CLOSE_STATE;
  handle->operation_mode=NONE_OPEN_CLOSE;
  handle->set_open_device=open_device;
  handle->set_close_device=close_device;
}


/**
  * @brief  Start the single io drive work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int Single_io_Drive_start(struct Single_io_Drive_t* handle)
{
	struct Single_io_Drive_t* target = head_handle;

	while(target) {
		if(target == handle) return -1;	//already exist.
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
	return 0;
}

/**
  * @brief  Stop the single io drive work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void Single_io_Drive_stop(struct Single_io_Drive_t* handle)
{
	struct Single_io_Drive_t** curr;

	for(curr = &head_handle; *curr; ) {
		struct Single_io_Drive_t* entry = *curr;
		if (entry == handle) {
			*curr = entry->next;
			//free(entry);
			return;
		} else{
			curr = &entry->next;
    }
	}
}


/**
  * @brief  Get the current single io device status.
  * @param  device_id: the single io device ID.
  * @retval Drive state.
  */
Device_state_e Get_Single_io_Drive_State(uint8_t device_id)
{
  struct Single_io_Drive_t** curr;

	for(curr = &head_handle; *curr; ) {
    if((*curr)->device_id==device_id){
      return (*curr)->device_state;
    }else{
      curr=&(*curr)->next;
    }
	}

	return DEVICE_INEXISTENT_STATE;
}

/**
  * @brief  Set Device Status.
  * @param  device_id: operation the single io device ID.
  * @param  mode: Set operation mode.
  * @param  delay_time: Delayed operation time.
  * @param  loop_open_time: Cycle mode open time.
  * @param  loop_close_time: Cycle mode close time.
  * @retval 0: Succeed. -1: Drive inexistent.
  */
int Set_Single_io_Drive_State(uint8_t device_id, Device_Operation_State_e mode, uint16_t delay_time, uint16_t loop_open_time, uint16_t loop_close_time )
{
	struct Single_io_Drive_t* target;

	for(target=head_handle; target; target=target->next){
    if(target->device_id==device_id){
      switch (mode){
        case OPEN_DEVICE_NO_DELAY:
          target->set_open_device();
          target->device_state=DEVICE_OPEN_STATE;
          target->operation_mode=NONE_OPEN_CLOSE;
          break;
        
        case CLOSE_DEVICE_NO_DELAY:
          target->set_close_device();
          target->device_state=DEVICE_CLOSE_STATE;
          target->operation_mode=NONE_OPEN_CLOSE;
          break;
        
        case OPEN_DEVICE_DELAY:
          target->delay_operation_time=delay_time/SINGLE_IO_TICKS_INTERVAL;
          target->time_start_flag=NOW_COUNT_STOP;
          target->operation_mode=OPEN_DEVICE_DELAY;
          break;

        case CLOSE_DEVICE_DELAY:
          target->delay_operation_time=delay_time/SINGLE_IO_TICKS_INTERVAL;
          target->time_start_flag=NOW_COUNT_STOP;
          target->operation_mode=CLOSE_DEVICE_DELAY;
          break;

        case OPEN_AND_CLOSE_LOOP_NO_DELAY:
          target->loop_Open_time=loop_open_time/SINGLE_IO_TICKS_INTERVAL;
          target->loop_Close_time=loop_close_time/SINGLE_IO_TICKS_INTERVAL;
          target->loop_count=target->loop_Open_time;
          target->time_start_flag=NOW_COUNT_STOP;
          target->set_open_device();
          target->device_state=DEVICE_OPEN_STATE;
          target->operation_mode=NOW_LOOP_MODE;
          break;

        case CLOSE_OPEN_AND_LOOP_NO_DELAY:
          target->loop_Open_time=loop_open_time/SINGLE_IO_TICKS_INTERVAL;
          target->loop_Close_time=loop_close_time/SINGLE_IO_TICKS_INTERVAL;
          target->loop_count=target->loop_Close_time;
          target->time_start_flag=NOW_COUNT_STOP;
          target->set_close_device();
          target->device_state=DEVICE_CLOSE_STATE;
          target->operation_mode=NOW_LOOP_MODE;
          break;

        case OPEN_AND_CLOSE_LOOP_DELAY:
          target->delay_operation_time=delay_time/SINGLE_IO_TICKS_INTERVAL;
          target->loop_Open_time=loop_open_time/SINGLE_IO_TICKS_INTERVAL;
          target->loop_Close_time=loop_close_time/SINGLE_IO_TICKS_INTERVAL;
          target->time_start_flag=NOW_COUNT_STOP;
          target->operation_mode=OPEN_AND_CLOSE_LOOP_DELAY;
          break;

        case CLOSE_OPEN_AND_LOOP_DELAY:
          target->delay_operation_time=delay_time/SINGLE_IO_TICKS_INTERVAL;
          target->loop_Open_time=loop_open_time/SINGLE_IO_TICKS_INTERVAL;
          target->loop_Close_time=loop_close_time/SINGLE_IO_TICKS_INTERVAL;
          target->time_start_flag=NOW_COUNT_STOP;
          target->operation_mode=CLOSE_OPEN_AND_LOOP_DELAY;
          break;

        default:
          target->operation_mode=NONE_OPEN_CLOSE;
          break;
      }

      return 0;
    }
	}

  return -1;
}

/**
  * @brief  Single io driver core function, driver state machine.
  * @param  handle: the Single io driver handle strcut.
  * @retval None
  */
void Single_io_Drive_handler(struct Single_io_Drive_t* handle)
{
  switch(handle->operation_mode){ 
    case OPEN_DEVICE_DELAY:
      if(handle->time_start_flag==NOW_COUNT_STOP){
        handle->time_start_flag=NOW_COUNT_START;
      }else{
        if((--handle->delay_operation_time)==0){
          handle->set_open_device();
          handle->device_state=DEVICE_OPEN_STATE;
          handle->operation_mode=NONE_OPEN_CLOSE;
        }
      }
      break;

    case CLOSE_DEVICE_DELAY:
      if(handle->time_start_flag==NOW_COUNT_STOP){
        handle->time_start_flag=NOW_COUNT_START;
      }else{
        if((--handle->delay_operation_time)==0){
          handle->set_close_device();
          handle->device_state=DEVICE_CLOSE_STATE;
          handle->operation_mode=NONE_OPEN_CLOSE;
        }
      }
      break;

    case OPEN_AND_CLOSE_LOOP_DELAY:
      if(handle->time_start_flag==NOW_COUNT_STOP){
        handle->time_start_flag=NOW_COUNT_START;
      }else{
        if((--handle->delay_operation_time)==0){
          handle->set_open_device();
          handle->device_state=DEVICE_OPEN_STATE;
          handle->loop_count=handle->loop_Open_time;
          handle->operation_mode=NOW_LOOP_MODE;
        }
      }
      break;

    case CLOSE_OPEN_AND_LOOP_DELAY:
      if(handle->time_start_flag==NOW_COUNT_STOP){
        handle->time_start_flag=NOW_COUNT_START;
      }else{
        if((--handle->delay_operation_time)==0){
          handle->set_close_device();
          handle->device_state=DEVICE_CLOSE_STATE;
          handle->loop_count=handle->loop_Close_time;
          handle->operation_mode=NOW_LOOP_MODE;
        }
      }
      break;
    
    case NOW_LOOP_MODE:
      if((--handle->loop_count)==0){
        if(handle->device_state==DEVICE_OPEN_STATE){
          handle->set_close_device();
          handle->device_state=DEVICE_CLOSE_STATE;
          handle->loop_count=handle->loop_Close_time;
        }else{
          handle->set_open_device();
          handle->device_state=DEVICE_OPEN_STATE;
          handle->loop_count=handle->loop_Open_time;
        }
      }
      break;

    default:
      handle->operation_mode=NONE_OPEN_CLOSE;
      break;
  }
}


/**
  * @brief  background ticks, timer repeat invoking interval x ms.
  * @param  None.
  * @retval None
  */
void Single_io_Drive_ticks(void)
{
	struct Single_io_Drive_t* target;
	for(target=head_handle; target; target=target->next) {
		Single_io_Drive_handler(target);
	}
}




