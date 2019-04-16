#ifndef __DOOR_H
#define __DOOR_H
#include "main.h"
#include "global.h"

typedef enum
{
  DOORSTATE_CLOSED = 0U,
	DOORSTATE_LOCKREADY,
	DOORSTATE_LOCKOPENED,
	DOORSTATE_DOOROPENED,
	DOORSTATE_DOORCLOSED,
} DOOR_State;


BOOL OpenDoor(void);
BOOL CloseDoor(void);
BOOL CheckDoorOpened(void);
BOOL CheckDoorClosed(void);
DOOR_State GetCurState(void);

#endif
