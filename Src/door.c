#include "door.h"
#include "main.h"




#define DO_CLOSE GPIO_PIN_SET
#define DO_OPEN GPIO_PIN_RESET
#define LOCK_CLOSE GPIO_PIN_RESET
#define LOCK_OPEN GPIO_PIN_SET
#define DOOR_CLOSE GPIO_PIN_RESET
#define DOOR_OPEN GPIO_PIN_SET


#define OpenRelay(n) HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, n)
#define Lock1 HAL_GPIO_ReadPin(Lock1_GPIO_Port, Lock1_Pin)
#define Lock2 HAL_GPIO_ReadPin(Lock2_GPIO_Port, Lock2_Pin)
#define Door1 HAL_GPIO_ReadPin(Door1_GPIO_Port, Door1_Pin)
#define Door2 HAL_GPIO_ReadPin(Door2_GPIO_Port, Door2_Pin)


DOOR_State curState=DOORSTATE_CLOSED;


DOOR_State GetCurState()
{
	return curState;
}


//开门，打开继电器，并检测锁的状态是否已经打开
BOOL OpenDoor()
{	
	LOG("Open Door");
	int count=0;
	OpenRelay(DO_OPEN);
	HAL_Delay(300);
	while(count++<10)
	{
		HAL_Delay(10);
		if(Lock1==LOCK_OPEN)
		{
			HAL_Delay(10);
			if(Lock1==LOCK_OPEN)
			{
				curState=DOORSTATE_LOCKOPENED;
				return TRUE;
			}
		}
	}
	return FALSE;
}
//关门，关闭继电器
BOOL CloseDoor()
{
	LOG("Close Door");
	int count=0;
	curState=DOORSTATE_CLOSED;
	OpenRelay(DO_CLOSE);
	HAL_Delay(300);
	while(count++<10)
	{
		HAL_Delay(10);
		if(Lock1==LOCK_CLOSE)
		{
			HAL_Delay(10);
			if(Lock1==LOCK_CLOSE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}
BOOL CheckDoorOpened()
{
	if(Door1==DOOR_OPEN)
	{
		HAL_Delay(10);
		if(Door1==DOOR_OPEN)
		{
			LOG("Door opened");
			curState=DOORSTATE_DOOROPENED;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CheckDoorClosed()
{
	if(Door1==DOOR_CLOSE)
	{
		HAL_Delay(10);
		if(Door1==DOOR_CLOSE)
		{
			LOG("Door Closed");
			curState=DOORSTATE_DOORCLOSED;
			return TRUE;
		}
	}
	return FALSE;
}


