// Task 2: Board A (sender)
// Wiring: A D11 (TX) -> B D10 (RX), A D10 (RX) <- B D11 (TX), GND -> GND
#include <SoftwareSerial.h>

SoftwareSerial link(10, 11);             // RX = D10, TX = D11

const char* const messages[] = {"Hello", "Test", "Loop"};
const uint8_t NUM_MESSAGES = sizeof(messages) / sizeof(messages[0]);

const unsigned long SEND_INTERVAL = 1000;   // ms between messages

unsigned long lastSend = 0;
uint8_t msgIndex = 0;
unsigned int counter = 0;

void setup()
{
    link.begin(9600);                    // link to Board B
    Serial.begin(9600);                  // USB Serial Monitor for debugging
    Serial.println(F("Sender ready"));
}

void loop()
{
    if (millis() - lastSend >= SEND_INTERVAL)
    {
        lastSend += SEND_INTERVAL;       // fixed period, no drift

        // Frame: text, space, counter, newline  e.g. "Hello 12\n"
        link.print(messages[msgIndex]);
        link.print(' ');
        link.print(counter);
        link.print('\n');

        Serial.print(F("Sent: "));
        Serial.print(messages[msgIndex]);
        Serial.print(' ');
        Serial.println(counter);

        msgIndex = (msgIndex + 1) % NUM_MESSAGES;
        counter++;
    }
}
