#include "button.h"

static uint8_t prevState[4] = {1,1,1,1};

void Button_Task(Queue_t *queue)
{
    GPIO_PinState current;

    // -------- Floor 1 --------
    current = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);

    if(prevState[0] == GPIO_PIN_SET && current == GPIO_PIN_RESET)
    {
        Enqueue(queue,1);
    }

    prevState[0] = current;

    // -------- Floor 2 --------
    current = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);

    if(prevState[1] == GPIO_PIN_SET && current == GPIO_PIN_RESET)
    {
        Enqueue(queue,2);
    }

    prevState[1] = current;

    // -------- Floor 3 --------
    current = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2);

    if(prevState[2] == GPIO_PIN_SET && current == GPIO_PIN_RESET)
    {
        Enqueue(queue,3);
    }

    prevState[2] = current;

    // -------- Floor 4 --------
    current = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);

    if(prevState[3] == GPIO_PIN_SET && current == GPIO_PIN_RESET)
    {
        Enqueue(queue,4);
    }

    prevState[3] = current;
}
