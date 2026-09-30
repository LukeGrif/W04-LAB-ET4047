# UART Communication and Data Integrity (ET4047 · Week 04 · Option 2)

**Module:** ET4047 · **Author:** Luke Griffin
**University of Limerick — Department of Electronic & Computer Engineering**

📄 **View the full lab document online:** https://lukegrif.github.io/W04-LAB-ET4047/viewer.html?lab=uart

> This is **Option 2** of the three Week 04 labs. See the [Week 04 overview](../README.md).

---

## Overview

UART (Universal Asynchronous Receiver/Transmitter) is the simplest and most widely
used way for two chips to talk. Data travels one bit at a time down a single wire
in each direction, with no shared clock; both sides agree on the **baud rate** in
advance.

In this lab you send data from an Arduino back to itself, then between two
Arduinos, and finally add a **SHA-1 hash** to every message so the receiver can
detect whether anything was corrupted on the way.

### Learning outcomes

- Describe a UART frame (start bit, data bits, stop bit) and calculate
  transmission time from the baud rate.
- Wire a loopback test and a two-board TX/RX link with a common ground.
- Explain how non-blocking `millis()` timing and a line-buffered receiver work,
  and why they are needed.
- Use a hash to check message integrity, and explain what a hash can and cannot
  protect against.

### Equipment

| Item | Qty |
|------|-----|
| Arduino Nano (classic) or Nano Every, on a breakout shield | 2 |
| 16×2 LCD with I2C backpack (address `0x20`), **or** Adafruit RGB LCD shield | 1–2 |
| Jumper wires | ~10 |
| USB cables | 2 |
| Push button (Task 3 only) | 1 |

---

## How UART works

When idle the line is held high. Each byte is sent as a 10-bit frame: a low
**start bit**, 8 data bits (least-significant bit first), then a high **stop bit**.

| idle | Start | D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7 | Stop | idle |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 1 | 1 |

*Sending `'A'` (0x41). At 9600 baud each bit lasts ≈ 104 µs, so one frame takes
about 1.04 ms.*

For this to work, both sides must use the **same baud rate**, **TX must go to RX**
(the wires cross over), and the **grounds must be connected**.

---

## Tasks

| # | Task | What you do |
|---|------|-------------|
| 1 | UART loopback on a single board | Wire D1 (TX) to D0 (RX) and check that everything sent comes back |
| 2 | Communication between two boards | Board A sends, Board B receives with a line-buffered receiver |
| 3 | Message integrity with SHA-1 | Append a hash to each message; a tamper button corrupts messages on purpose |
| Ext | Acknowledgements (ACK/NACK) | The receiver replies and the sender retries failed messages |

### Submission checklist

- **Task 1:** LCD photo and answers to 1b, 1c and 1d.
- **Task 2:** Serial Monitor output from both boards, the completed "break it"
  table, and answers to 2c.
- **Task 3:** an OK and a FAIL output from both boards, and answers to 3b and 3c.
- **Extension (optional):** Board A output showing a successful retry, and answers
  to E2.

---

## Solutions

Each sketch is in its own folder so it opens directly in the Arduino IDE. The
sketches that use SHA-1 include their own copy of `sha1_helper.h` (Appendix A of
the lab document), and the sketches that use the LCD include `lab_lcd.h`
(Appendix B).

| Task | Board A | Board B |
|------|---------|---------|
| 1 — Loopback | [`Task1_Loopback_Solution`](Solutions/Task1_Loopback_Solution/Task1_Loopback_Solution.ino) (single board, D1 → D0) | — |
| 2 — Two boards | [`Task2_Sender_Solution`](Solutions/Task2_Sender_Solution/Task2_Sender_Solution.ino) | [`Task2_Receiver_Solution`](Solutions/Task2_Receiver_Solution/Task2_Receiver_Solution.ino) |
| 3 — SHA-1 integrity | [`Task3_Sender_Solution`](Solutions/Task3_Sender_Solution/Task3_Sender_Solution.ino) | [`Task3_Receiver_Solution`](Solutions/Task3_Receiver_Solution/Task3_Receiver_Solution.ino) |
| Extension — ACK/NACK | [`Extension_Sender_ACK`](Solutions/Extension_Sender_ACK/Extension_Sender_ACK.ino) | [`Extension_Receiver_ACK`](Solutions/Extension_Receiver_ACK/Extension_Receiver_ACK.ino) |

**Either LCD works.** Every sketch that uses the LCD includes
[`lab_lcd.h`](Solutions/Task2_Receiver_Solution/lab_lcd.h), which checks the I2C
bus in `lcd.init()` and drives whichever display is plugged in:

| Display | Chip | How it is recognised |
|---------|------|----------------------|
| 16×2 LCD with I2C backpack | PCF8574 at 0x20–0x27 (or PCF8574A at 0x38–0x3F) | answers on the bus, but has no registers to read back |
| Adafruit RGB LCD shield | MCP23017 at 0x20 | a test value written to one of its registers reads back unchanged |

Both displays can sit at address 0x20, so the address alone cannot tell them
apart; the register test does. The receivers print the result to the Serial
Monitor (for example `LCD: Adafruit RGB LCD shield`). The shield plugs straight
onto the Uno-style breakout and uses the same SDA (A4) and SCL (A5) pins, so no
wiring is needed.

**Libraries:** `SoftwareSerial` and `Wire` come with the Arduino IDE. Install
**both** LCD libraries from the Library Manager, whichever display you have:
**LiquidCrystal I2C** (by Frank de Brabander) and **Adafruit RGB LCD Shield
Library** (by Adafruit).

**Tip for the extension:** the sender retries straight after each NACK, so all
five retries happen within roughly 0.35 s. To see a successful retry, *tap* the
tamper button briefly; holding it down for longer makes the sender give up on
that message.

---

## Repository Contents

| Path | Description |
|------|-------------|
| `W04_LAB_UART_Communication_and_Data_Integrity.pdf` | The lab document |
| `W04 LAB UART Communication and Data Integrity.docx` | Editable Word version |
| `Solutions/` | Solution sketches for Tasks 1–3 and the extension |
