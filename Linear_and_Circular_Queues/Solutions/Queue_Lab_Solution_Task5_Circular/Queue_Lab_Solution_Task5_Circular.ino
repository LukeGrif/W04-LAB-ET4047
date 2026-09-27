
#include <Arduino.h>

// ============================================================
// Linear Queue converted to circular - SOLUTION Task 5
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
    // TASK 5: full means every slot holds an element
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

    // TASK 5: rear wraps from the last slot back to slot 0
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
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

    // TASK 5: no shifting needed - step front back one slot,
    // wrapping from slot 0 to the last slot
    q->front = (q->front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    q->items[q->front] = value;

    // If this is the only element, rear must point at it too
    if (isEmpty(q))
    {
        q->rear = q->front;
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

    int item = q->items[q->front];

    // TASK 5: front wraps from the last slot back to slot 0
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->count--;

    // Reset queue when the last element is removed
    if (q->count == 0)
    {
        q->front = 0;
        q->rear = -1;
    }

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
// Task 2 / 5: print the queue state (wrap-around aware)
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
        // How far is slot i from the front, walking forwards with wrap-around?
        int offset = (i - q->front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;

        if (offset < q->count)
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
