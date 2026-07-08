#ifndef __QUEUE_H
#define __QUEUE_H

#include "stdint.h"

#define QUEUE_SIZE 10

typedef struct
{
    uint8_t data[QUEUE_SIZE];
    uint8_t front;
    uint8_t rear;
    uint8_t count;

}Queue_t;

void Queue_Init(Queue_t *q);

uint8_t Queue_IsEmpty(Queue_t *q);

uint8_t Queue_IsFull(Queue_t *q);

uint8_t Enqueue(Queue_t *q,uint8_t value);

uint8_t Dequeue(Queue_t *q,uint8_t *value);

#endif
