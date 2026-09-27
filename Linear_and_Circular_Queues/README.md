# Linear and Circular Queues (ET4047 · Week 04 · Option 3)

**Module:** ET4047 · **Author:** Luke Griffin
**University of Limerick — Department of Electronic & Computer Engineering**

📄 **View the full lab document online:** https://lukegrif.github.io/W04-LAB-ET4047/viewer.html?lab=queues

> This is **Option 3** of the three Week 04 labs. See the [Week 04 overview](../README.md).

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

## Repository Contents

| Path | Description |
|------|-------------|
| `W04_LAB_Linear_and_Circular_Queues.pdf` | The lab document |
| `W04 LAB Linear and Circular Queues.docx` | Editable Word version |

*The example sketches and task code will be added to this folder.*
