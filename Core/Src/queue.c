#include "queue.h"

void Queue_Init(Queue_t *q)
{
    q->front=0;
    q->rear=0;
    q->count=0;
}

uint8_t Queue_IsEmpty(Queue_t *q)
{
    return (q->count==0);
}

uint8_t Queue_IsFull(Queue_t *q)
{
    return (q->count==QUEUE_SIZE);
}

uint8_t Enqueue(Queue_t *q,uint8_t value)
{
    if(Queue_IsFull(q))
        return 0;

    q->data[q->rear]=value;

    q->rear++;

    if(q->rear>=QUEUE_SIZE)
        q->rear=0;

    q->count++;

    return 1;
}

uint8_t Dequeue(Queue_t *q,uint8_t *value)
{
    if(Queue_IsEmpty(q))
        return 0;

    *value=q->data[q->front];

    q->front++;

    if(q->front>=QUEUE_SIZE)
        q->front=0;

    q->count--;

    return 1;
}
