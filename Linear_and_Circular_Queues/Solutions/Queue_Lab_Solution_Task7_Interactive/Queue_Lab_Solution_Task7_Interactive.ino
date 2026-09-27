
#include <Arduino.h>

// ============================================================
// Circular queue with interactive Serial commands - SOLUTION Task 7
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
// Task 7: interactive queue over Serial
// ------------------------------------------------------------

void printHelp()
{
    Serial.println("Commands:");
    Serial.println("  e <n>  enqueue n");
    Serial.println("  f <n>  enqueueToOppositeEnd n");
    Serial.println("  d      dequeue");
    Serial.println("  p      peek");
    Serial.println("  c      clear (destroyQueue)");
    Serial.println();
}


Queue myQueue;


void setup()
{
    Serial.begin(9600);

    initializeQueue(&myQueue);

    Serial.println("Interactive queue ready.");
    printHelp();
    printQueue(&myQueue);
}


void loop()
{
    // Wait until a line has been typed
    if (Serial.available() == 0)
    {
        return;
    }

    String line = Serial.readStringUntil('\n');
    line.trim();                      // remove spaces and any '\r'

    if (line.length() == 0)
    {
        return;
    }

    Serial.print("> ");
    Serial.println(line.c_str());

    char cmd = line.charAt(0);

    // Everything after the command letter is the number (if any)
    int value = line.substring(1).toInt();

    if (cmd == 'e')
    {
        enqueue(&myQueue, value);
    }
    else if (cmd == 'f')
    {
        enqueueToOppositeEnd(&myQueue, value);
    }
    else if (cmd == 'd')
    {
        dequeue(&myQueue);            // dequeue() prints the value itself
    }
    else if (cmd == 'p')
    {
        if (!isEmpty(&myQueue))
        {
            Serial.print("Front element: ");
        }
        int front = peek(&myQueue);
        if (!isEmpty(&myQueue))
        {
            Serial.println(front);
        }
    }
    else if (cmd == 'c')
    {
        destroyQueue(&myQueue);
    }
    else
    {
        Serial.println("Unknown command.");
        printHelp();
    }

    printQueue(&myQueue);
    Serial.println();
}
