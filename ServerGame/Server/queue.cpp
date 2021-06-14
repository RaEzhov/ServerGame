#include <winsock.h>
#include "queue.h"

SOCKET peek (struct queue* q){
    return q->clientsArray[q->front];
}

int isEmpty (struct queue* q){
    return q->itemCount == 0;
}

int isFull (struct queue* q){
    return q->itemCount == MAX;
}

int size (struct queue* q){
    return q->itemCount;
}

void insert (SOCKET client, struct queue* q){
    if (!isFull (q)){
        if (q->rear == MAX - 1){
            q->rear = -1;
        }
        q->clientsArray[++q->rear] = client;
        q->itemCount++;
    }
}

SOCKET removeData (struct queue* q){
    int data = q->clientsArray[q->front++];
    if (q->front == MAX){
        q->front = 0;
    }
    q->itemCount--;
    return data;
}