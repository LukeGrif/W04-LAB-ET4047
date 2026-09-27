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
| 16×2 LCD with I2C backpack (address `0x20`) | 1–2 |
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

## Repository Contents

| Path | Description |
|------|-------------|
| `W04_LAB_UART_Communication_and_Data_Integrity.pdf` | The lab document |
| `W04 LAB UART Communication and Data Integrity.docx` | Editable Word version |

*Code files for each task will be added to this folder.*
