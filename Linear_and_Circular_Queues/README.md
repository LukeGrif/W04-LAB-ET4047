# Linear and Circular Queues (ET4047 · Week 04 · Option 2)

**Module:** ET4047 · **Author:** Luke Griffin
**University of Limerick — Department of Electronic & Computer Engineering**

📄 **View the full lab document online:** https://lukegrif.github.io/W04-LAB-ET4047/viewer.html?lab=queues

> This is **Option 2** of the two Week 04 labs. See the [Week 04 overview](../README.md).

---

## Overview

A **queue** is a First-In, First-Out (FIFO) data structure: elements join at the
**rear** and leave from the **front**. In this lab you run two array-based queue
implementations on an Arduino Nano in the [Wokwi](https://wokwi.com) simulator,
work through how each function operates, and then fix a real flaw in the linear
version.

**The flaw:** in the linear queue, every `dequeue()` permanently uses up a slot at
the start of the array. Once `rear` reaches the last slot, the queue refuses new
elements even when most of the array is empty. By the end of this lab your queue
reuses freed space, so it can be filled and emptied indefinitely.

### Learning outcomes

- Explain how `front`, `rear` and `count` describe the state of an array-based queue.
- Trace `enqueue()`, `dequeue()`, `peek()` and `enqueueToOppositeEnd()` by hand
  and on hardware.
- Demonstrate why a linear queue wastes space after `dequeue()`.
- Fix the problem two ways: by shifting elements, and by circular (modulo) indexing.
- Compare the time cost of each approach using Big-O notation.

### What you need

- A web browser and Wokwi: **wokwi.com → New Project → Arduino Nano**.
- The two example sketches: `Linear_Queue_Example.ino` and
  `Circular_Queue_Example.ino`.

---

## Tasks

| # | Task | What you do |
|---|------|-------------|
| 1 | Run and compare both examples | Run both sketches and explain why their identical output hides the difference |
| 2 | Add a `printQueue()` function | Print `front`, `rear`, `count` and every array slot |
| 3 | Expose the problem | Shrink the queue to 5 and show `enqueue()` rejecting values while slots are free |
| 4 | Fix 1: shift elements on dequeue | Slide the remaining elements left so `front` stays at 0 |
| 5 | Fix 2: convert to a circular queue | Wrap `front` and `rear` with the modulo operator |
| 6 | Stress test | Push many elements through a queue that is never allowed to empty, to prove the fix |
| 7 | Extension: interactive queue over Serial | Drive the queue with commands typed into the Serial Monitor |

---

## Solutions

Each sketch is in its own folder. Paste it into `sketch.ino` in a Wokwi Arduino
Nano project (or open it in the Arduino IDE) and watch the Serial Monitor at
9600 baud.

| Sketch | Covers |
|--------|--------|
| [`Queue_Lab_Solution_Task4_Shift`](Solutions/Queue_Lab_Solution_Task4_Shift/Queue_Lab_Solution_Task4_Shift.ino) | Task 2 `printQueue()`, Task 4 shift-on-dequeue fix, Task 3 test sequence and Task 6 stress test |
| [`Queue_Lab_Solution_Task5_Circular`](Solutions/Queue_Lab_Solution_Task5_Circular/Queue_Lab_Solution_Task5_Circular.ino) | Task 5 circular (modulo) queue with a wrap-around-aware `printQueue()`, Task 3 test sequence and Task 6 stress test |
| [`Queue_Lab_Solution_Task7_Interactive`](Solutions/Queue_Lab_Solution_Task7_Interactive/Queue_Lab_Solution_Task7_Interactive.ino) | Task 7 interactive circular queue: type `e <n>`, `f <n>`, `d`, `p` or `c` in the Serial Monitor |

Expected final line of the Task 3 test sequence:

| Version | Output |
|---------|--------|
| Task 4 (shift) | `front=0 rear=4 count=5  \| [30] [40] [50] [60] [70]` |
| Task 5 (circular) | `front=2 rear=1 count=5  \| [60] [70] [30] [40] [50]` |

Both versions finish the stress test with `Stress test finished. Failures: 0`.

---

## Repository Contents

| Path | Description |
|------|-------------|
| `W04_LAB_Linear_and_Circular_Queues.pdf` | The lab document |
| `W04 LAB Linear and Circular Queues.docx` | Editable Word version |
| `Solutions/` | Solution sketches for Tasks 4, 5 and 7 |
