// Extension: Board A (sender with SHA-1, tamper button and ACK/NACK retries)
// Same wiring as Task 3. Listens for replies on D10 (RX) from B's D11 (TX).
#include <SoftwareSerial.h>
#include "sha1_helper.h"

SoftwareSerial link(10, 11);             // RX = D10, TX = D11

const char* const messages[] = {"Testing", "Embedded", "Software"};
const uint8_t NUM_MESSAGES = sizeof(messages) / sizeof(messages[0]);

const uint8_t BUTTON_PIN = 2;
const unsigned long SEND_INTERVAL = 2000;   // ms between new messages
const unsigned long ACK_TIMEOUT = 500;      // ms to wait for a reply
const uint8_t MAX_RETRIES = 5;              // resends before giving up
const uint8_t MAX_MSG = 31;

enum State : uint8_t { IDLE, WAITING_FOR_REPLY };
State state = IDLE;

unsigned long lastSend = 0;              // when the current message was finished
unsigned long sentAt = 0;                // when the last frame went out
uint8_t msgIndex = 0;
uint8_t retries = 0;

char reply[8];                           // "ACK" / "NACK" plus room to spare
uint8_t replyLen = 0;

// Hash and send messages[msgIndex] (corrupted while the button is held)
void sendFrame()
{
    const char* msg = messages[msgIndex];

    char hexHash[41];
    sha1_hex(msg, hexHash);

    char corrupted[MAX_MSG + 1];
    bool tamper = (digitalRead(BUTTON_PIN) == LOW);
    if (tamper)
    {
        strncpy(corrupted, msg, MAX_MSG);
        corrupted[MAX_MSG] = '\0';
        corrupted[0] ^= 0x01;            // flip one bit
        msg = corrupted;
    }

    link.print(msg);
    link.print('|');
    link.print(hexHash);
    link.print('\n');

    Serial.print(tamper ? F("Sent (TAMPERED): ") : F("Sent: "));
    Serial.println(msg);

    sentAt = millis();
    state = WAITING_FOR_REPLY;
}

// Finished with this message (delivered or given up): move on
void nextMessage()
{
    msgIndex = (msgIndex + 1) % NUM_MESSAGES;
    retries = 0;
    lastSend = millis();                 // next message SEND_INTERVAL from now
    state = IDLE;
}

// Resend the same message, or give up after MAX_RETRIES
void retry()
{
    if (retries < MAX_RETRIES)
    {
        retries++;
        Serial.print(F("  Retry "));
        Serial.println(retries);
        sendFrame();
    }
    else
    {
        Serial.println(F("  Giving up on this message"));
        nextMessage();
    }
}

void handleReply(const char* r)
{
    if (state != WAITING_FOR_REPLY)
    {
        return;                          // late or unexpected reply: ignore it
    }

    if (strcmp(r, "ACK") == 0)
    {
        Serial.print(F("  ACK"));
        if (retries > 0)
        {
            Serial.print(F(" after "));
            Serial.print(retries);
            Serial.print(F(" retries"));
        }
        Serial.println();
        nextMessage();
    }
    else
    {
        Serial.println(F("  NACK"));
        retry();
    }
}

void setup()
{
    link.begin(9600);
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.println(F("Sender ready"));
}

void loop()
{
    // 1. Time to send a new message?
    if (state == IDLE && millis() - lastSend >= SEND_INTERVAL)
    {
        sendFrame();
    }

    // 2. Collect reply characters into a line
    while (link.available())
    {
        char c = link.read();
        if (c == '\r')
        {
            continue;
        }
        if (c == '\n')
        {
            reply[replyLen] = '\0';
            handleReply(reply);
            replyLen = 0;
        }
        else if (replyLen < sizeof(reply) - 1)
        {
            reply[replyLen++] = c;
        }
    }

    // 3. No reply in time? Treat it like a NACK
    if (state == WAITING_FOR_REPLY && millis() - sentAt >= ACK_TIMEOUT)
    {
        Serial.println(F("  Timeout"));
        retry();
    }
}
