// Task 2: Board B (receiver with LCD)
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial link(10, 11);             // RX = D10, TX = D11

const uint8_t LCD_COLS = 16;
LiquidCrystal_I2C lcd(0x20, LCD_COLS, 2);

const uint8_t MAX_LEN = 32;              // longest message we accept
char buffer[MAX_LEN + 1];                // +1 for the '\0' terminator
uint8_t len = 0;                         // characters stored so far

// Write text on one LCD row and blank the rest of that row.
// Faster and flicker-free compared with lcd.clear() + print.
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

// Show one complete message on the Serial Monitor and the LCD
void showMessage(const char* msg)
{
    Serial.print(F("Received: "));
    Serial.println(msg);

    lcdPrintLine(0, "Received:");
    lcdPrintLine(1, msg);
}

void setup()
{
    link.begin(9600);
    Serial.begin(9600);

    lcd.init();
    lcd.backlight();
    lcdPrintLine(0, "Waiting...");
}

void loop()
{
    while (link.available())
    {
        char c = link.read();

        if (c == '\r')
        {
            continue;                    // ignore carriage returns
        }

        if (c == '\n')                   // end of message
        {
            buffer[len] = '\0';          // turn the buffer into a C string
            showMessage(buffer);
            len = 0;                     // get ready for the next message
        }
        else if (len < MAX_LEN)          // store it if there is room
        {
            buffer[len++] = c;
        }
    }
}
