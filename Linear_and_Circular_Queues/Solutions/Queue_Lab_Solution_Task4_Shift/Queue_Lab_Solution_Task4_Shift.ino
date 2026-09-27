
#include <Arduino.h>

// ============================================================
// Linear Queue - SOLUTION Task 4 (shift elements on dequeue)
// ============================================================

#define MAX_QUEUE_SIZE 5

typedef struct
{
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
} Queue;


// ------------------------------------------------------------
// Initialize queue
// ------------------------------------------------------------

void initializeQueue(Queue *q)
{
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}


// ------------------------------------------------------------
// Check if queue is empty
// ------------------------------------------------------------

bool isEmpty(Queue *q)
{
    return q->count == 0;
}


// ------------------------------------------------------------
// Check if queue is full
// ------------------------------------------------------------

bool isFull(Queue *q)
{
    // TASK 4: full means every slot holds an element
    return q->count == MAX_QUEUE_SIZE;
}


// ------------------------------------------------------------
// Add element to rear of queue
// ------------------------------------------------------------

bool enqueue(Queue *q, int value)
{
    if (isFull(q))
    {
        Serial.println("Error: Queue is full. Cannot enqueue.");
        return false;
    }

    q->rear++;
    q->items[q->rear] = value;
    q->count++;

    Serial.print("Enqueued: ");
    Serial.println(value);

    return true;
}


// ------------------------------------------------------------
// Add element to front of queue
// ------------------------------------------------------------

bool enqueueToOppositeEnd(Queue *q, int value)
{
    if (isFull(q))
    {
        Serial.println(
            "Error: Queue is full. Cannot enqueueToOppositeEnd."
        );

        return false;
    }

    // If the queue is empty
    if (isEmpty(q))
    {
        q->front = 0;
        q->rear = 0;
        q->items[q->front] = value;
    }
    else
    {
        // Shift all elements one position to the right
        for (int i = q->rear; i >= q->front; i--)
        {
            q->items[i + 1] = q->items[i];
        }

        // Insert the new element at the front
        q->items[q->front] = value;

        // Rear moves one position to the right
        q->rear++;
    }

    q->count++;

    Serial.print("Enqueued to opposite end: ");
    Serial.println(value);

    return true;
}


// ------------------------------------------------------------
// Remove element from front of queue
// ------------------------------------------------------------

int dequeue(Queue *q)
{
    if (isEmpty(q))
    {
        Serial.println(
            "Error: Queue is empty. Cannot dequeue."
        );

        return -1;
    }

    // front is always 0 in this version
    int item = q->items[q->front];

    // TASK 4: shift every remaining element one place to the left
    // so the freed slot moves to the rear of the array
    for (int i = q->front; i < q->rear; i++)
    {
        q->items[i] = q->items[i + 1];
    }

    q->rear--;
    q->count--;

    Serial.print("Dequeued: ");
    Serial.println(item);

    return item;
}


// ------------------------------------------------------------
// Get front element without removing it
// ------------------------------------------------------------

int peek(Queue *q)
{
    if (isEmpty(q))
    {
        Serial.println(
            "Error: Queue is empty. Cannot peek."
        );

        return -1;
    }

    return q->items[q->front];
}


// ------------------------------------------------------------
// Destroy / reset queue
// ------------------------------------------------------------

void destroyQueue(Queue *q)
{
    q->front = 0;
    q->rear = -1;
    q->count = 0;

    Serial.println("Queue destroyed.");
}



// ------------------------------------------------------------
// Task 2: print the queue state
// ------------------------------------------------------------

void printQueue(Queue *q)
{
    Serial.print("front=");
    Serial.print(q->front);
    Serial.print(" rear=");
    Serial.print(q->rear);
    Serial.print(" count=");
    Serial.print(q->count);
    Serial.print("  |");

    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        if (q->count > 0 && i >= q->front && i <= q->rear)
        {
            Serial.print(" [");
            Serial.print(q->items[i]);
            Serial.print("]");
        }
        else
        {
            Serial.print(" [--]");
        }
    }
    Serial.println();
}

// ------------------------------------------------------------
// Task 6: stress test
// ------------------------------------------------------------

void stressTest(Queue *q)
{
    int next = 1;       // next value to enqueue
    int expected = 1;   // value we expect dequeue() to return
    int failures = 0;

    // Keep one item in the queue so it never becomes empty
    enqueue(q, next++);

    for (int round = 1; round <= 10; round++)
    {
        for (int i = 0; i < 2; i++)
        {
            if (!enqueue(q, next++)) failures++;
        }
        for (int i = 0; i < 2; i++)
        {
            if (dequeue(q) != expected++) failures++;
        }
    }

    Serial.print("Stress test finished. Failures: ");
    Serial.println(failures);
}

// ============================================================
// Tests
// ============================================================

Queue myQueue;


void setup()
{
    Serial.begin(9600);

    // Task 3 test sequence
    initializeQueue(&myQueue);

    enqueue(&myQueue, 10);
    enqueue(&myQueue, 20);
    enqueue(&myQueue, 30);
    enqueue(&myQueue, 40);
    enqueue(&myQueue, 50);
    printQueue(&myQueue);
    Serial.println();

    dequeue(&myQueue);
    dequeue(&myQueue);
    printQueue(&myQueue);
    Serial.println();

    enqueue(&myQueue, 60);
    enqueue(&myQueue, 70);
    printQueue(&myQueue);
    Serial.println();

    // Task 6 stress test
    destroyQueue(&myQueue);
    stressTest(&myQueue);
}


void loop()
{
    // Nothing to do
}
