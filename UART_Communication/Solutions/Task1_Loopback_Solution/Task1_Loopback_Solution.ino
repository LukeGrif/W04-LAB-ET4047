// Task 1: UART loopback with 16-character line wrapping
// Wiring: jumper from D1 (TX) to D0 (RX)
// Works on both the classic Arduino Nano and the Nano Every.
#include <Wire.h>
#include "lab_lcd.h"                     // I2C backpack LCD or Adafruit RGB LCD shield

// Choose the hardware UART on D0/D1 for this board:
//   Nano Every   -> Serial1 (USB uses a separate channel)
//   Classic Nano -> Serial  (shared with USB: remove the jumper while uploading)
#if defined(ARDUINO_AVR_NANO_EVERY) || defined(HAVE_HWSERIAL1)
  #define LINK Serial1
#else
  #define LINK Serial
#endif

const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;
LabLCD lcd(LCD_COLS, LCD_ROWS);          // finds whichever display is plugged in

const char* const messages[] = {"Hello", "Test", "Loop"};
const uint8_t NUM_MESSAGES = sizeof(messages) / sizeof(messages[0]);

const unsigned long SEND_INTERVAL = 1000;   // ms

uint8_t col = 0;    // current LCD column
uint8_t row = 0;    // current LCD row

// Print one character, wrapping to the next row after 16 characters
void lcdPutChar(char c)
{
    if (col == LCD_COLS)
    {
        col = 0;
        row++;

        if (row == LCD_ROWS)          // both rows full: start again at the top
        {
            row = 0;
            lcd.clear();
        }
        lcd.setCursor(col, row);
    }

    lcd.write(c);
    col++;
}

void setup()
{
    LINK.begin(9600);                // hardware UART on D0 (RX) / D1 (TX)

    lcd.init();
    lcd.backlight();
    lcd.print(F("UART Loopback"));  // F() keeps the text in flash, not RAM
    delay(1000);
    lcd.clear();
}

void loop()
{
    static unsigned long lastSend = 0;
    static uint8_t msgIndex = 0;

    // Send the next message once per second (non-blocking, no drift)
    if (millis() - lastSend >= SEND_INTERVAL)
    {
        lastSend += SEND_INTERVAL;
        LINK.print(messages[msgIndex]);
        msgIndex = (msgIndex + 1) % NUM_MESSAGES;
    }

    // Display every byte that has come back in on RX
    while (LINK.available())
    {
        char c = LINK.read();
        if (c == '\r' || c == '\n')
        {
            continue;
        }
        lcdPutChar(c);
    }
}
