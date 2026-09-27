// Task 3: Board A (sender with SHA-1 and tamper button)
// Extra wiring: push button between D2 and GND
#include <SoftwareSerial.h>
#include "sha1_helper.h"

SoftwareSerial link(10, 11);             // RX = D10, TX = D11

const char* const messages[] = {"Testing", "Embedded", "Software"};
const uint8_t NUM_MESSAGES = sizeof(messages) / sizeof(messages[0]);

const uint8_t BUTTON_PIN = 2;
const unsigned long SEND_INTERVAL = 2000;   // ms
const uint8_t MAX_MSG = 31;                 // longest message we may tamper with

unsigned long lastSend = 0;
uint8_t msgIndex = 0;

void setup()
{
    link.begin(9600);
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);   // reads LOW while pressed
    Serial.println(F("Sender ready"));
}

void loop()
{
    if (millis() - lastSend >= SEND_INTERVAL)
    {
        lastSend += SEND_INTERVAL;

        const char* msg = messages[msgIndex];

        // 1. Hash the ORIGINAL message
        char hexHash[41];
        sha1_hex(msg, hexHash);

        // 2. While the button is held, send a copy with one bit flipped.
        //    Only copy when needed; normally the original is sent as-is.
        char corrupted[MAX_MSG + 1];
        bool tamper = (digitalRead(BUTTON_PIN) == LOW);
        if (tamper)
        {
            strncpy(corrupted, msg, MAX_MSG);
            corrupted[MAX_MSG] = '\0';
            corrupted[0] ^= 0x01;        // flip one bit, like a line error
            msg = corrupted;
        }

        // 3. Send the frame "MESSAGE|HASH\n"
        link.print(msg);
        link.print('|');
        link.print(hexHash);
        link.print('\n');

        Serial.print(tamper ? F("Sent (TAMPERED): ") : F("Sent: "));
        Serial.print(msg);
        Serial.print('|');
        Serial.println(hexHash);

        msgIndex = (msgIndex + 1) % NUM_MESSAGES;
    }
}
