#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "stdint.h"

extern volatile uint32_t systemTick;

extern volatile uint8_t buttonTaskFlag;
extern volatile uint8_t ledTaskFlag;
extern volatile uint8_t elevatorTaskFlag;

void Scheduler_Update(void);

#endif
