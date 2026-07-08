#include "scheduler.h"

volatile uint32_t systemTick = 0;

volatile uint8_t buttonTaskFlag = 0;
volatile uint8_t ledTaskFlag = 0;
volatile uint8_t elevatorTaskFlag = 0;

void Scheduler_Update(void)
{
    static uint32_t lastButton = 0;
    static uint32_t lastLED = 0;
    static uint32_t lastElevator = 0;

    if(systemTick - lastButton >= 10)
    {
        lastButton = systemTick;
        buttonTaskFlag = 1;
    }

    if(systemTick - lastLED >= 50)
    {
        lastLED = systemTick;
        ledTaskFlag = 1;
    }

    if(systemTick - lastElevator >= 100)
    {
        lastElevator = systemTick;
        elevatorTaskFlag = 1;
    }
}
