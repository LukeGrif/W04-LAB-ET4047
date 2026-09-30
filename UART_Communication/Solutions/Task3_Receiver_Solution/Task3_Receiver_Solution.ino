// Task 3: Board B (receiver with SHA-1 integrity check)
#include <SoftwareSerial.h>
#include <Wire.h>
#include "lab_lcd.h"                     // I2C backpack LCD or Adafruit RGB LCD shield
#include "sha1_helper.h"

SoftwareSerial link(10, 11);             // RX = D10, TX = D11

const uint8_t LCD_COLS = 16;
LabLCD lcd(LCD_COLS, 2);                 // finds whichever display is plugged in

const uint8_t MAX_LEN = 64;              // message + '|' + 40 hex chars fits
char buffer[MAX_LEN + 1];
uint8_t len = 0;
bool overflow = false;                   // true if the current line was too long

unsigned int okCount = 0;
unsigned int failCount = 0;

// Write text on one LCD row and blank the rest of that row
void lcdPrintLine(uint8_t row, const char* text)
{
    lcd.setCursor(0, row);
    uint8_t i = 0;
    for (; i < LCD_COLS && text[i] != '\0'; i++)
    {
        lcd.write(text[i]);
    }
    for (; i < LCD_COLS; i++)
    {
        lcd.write(' ');
    }
}

// Check one complete frame of the form "MESSAGE|HASH"
void handleFrame(char* frame)
{
    // 1. Split the frame at the '|' separator
    char* sep = strchr(frame, '|');
    if (sep == NULL)
    {
        Serial.println(F("Bad frame: no '|' separator"));
        return;
    }
    *sep = '\0';                         // end the message string here
    const char* msg = frame;             // text before '|'
    const char* rxHash = sep + 1;        // text after '|'

    // 2. Hash what actually arrived
    char calcHash[41];
    sha1_hex(msg, calcHash);

    // 3. Compare with the hash the sender attached
    bool ok = (strcmp(calcHash, rxHash) == 0);
    if (ok) okCount++; else failCount++;

    Serial.print(F("Message:  ")); Serial.println(msg);
    Serial.print(F("Received: ")); Serial.println(rxHash);
    Serial.print(F("Computed: ")); Serial.println(calcHash);
    Serial.println(ok ? F("Integrity: OK") : F("Integrity: FAIL"));
    Serial.println();

    char status[LCD_COLS + 1];
    snprintf(status, sizeof(status), "%s %u/%u", ok ? "OK  " : "FAIL",
             okCount, okCount + failCount);
    lcdPrintLine(0, msg);
    lcdPrintLine(1, status);
}

void setup()
{
    link.begin(9600);
    Serial.begin(9600);

    lcd.init();
    lcd.backlight();
    Serial.print(F("LCD: "));
    Serial.println(lcd.name());          // which display was found
    lcdPrintLine(0, "Waiting...");
}

void loop()
{
    while (link.available())
    {
        char c = link.read();

        if (c == '\r')
        {
            continue;
        }

        if (c == '\n')
        {
            buffer[len] = '\0';
            if (overflow)
            {
                Serial.println(F("Frame too long - discarded"));
            }
            else
            {
                handleFrame(buffer);
            }
            len = 0;
            overflow = false;
        }
        else if (len < MAX_LEN)
        {
            buffer[len++] = c;
        }
        else
        {
            overflow = true;
        }
    }
}
