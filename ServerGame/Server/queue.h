#ifndef QUEUE_H
#define QUEUE_H

#define MAX 50

struct queue{
    SOCKET clientsArray[MAX];
    int front;
    int rear;
    int itemCount;
};

SOCKET peek (struct queue* q);
int isEmpty (struct queue* q);
int isFull (struct queue* q);
int size (struct queue* q);
void insert (SOCKET client, struct queue* q);
SOCKET removeData (struct queue* q);

#endif//QUEUE_H