# Lab 3 — Beginner Assembly with the CPU Visual Simulator (ET4047)

**Module:** ET4047 · **Lab No.:** Lab 3 · **Author:** Luke Griffin
**University of Limerick — Department of Electronic & Computer Engineering**

📄 **View the full lab handout online:** https://lukegrif.github.io/W04-LAB-ET4047/

---

## Introduction

This lab introduces assembly programming on the
[CPU Visual Simulator](https://cpuvisualsimulator.github.io/). It follows the
approach from the W03 lecture: first understand how the CPU and memory are
organised, then write each program as **pseudo code**, translate it into
**assembler**, and check it by comparing the state of the CPU **before and after
execution**.

By the end of this lab you should be able to:

- describe what the PC, IR, Control Unit, MUX, ALU, ACC and Status Word do, and
  what travels on the address, data and control buses;
- explain how an instruction is stored in memory (opcode and operand) and why the
  PC goes up in steps of 2;
- tell the difference between **immediate** (`#X`) and **direct** (`X`) addressing
  and predict the result of each;
- read and write register-transfer notation such as `[X] -> ACC` and
  `ACC + #1 -> ACC`;
- translate IF-THEN-ELSE and WHILE-DO pseudo code into assembler using `CMP` and
  conditional jumps.

**How this lab works.** Part A explains how the simulator works — read it first
and keep it open as a reference. Part B contains six tasks. Each task gives the
pseudo code, the assembler (a worked example) and a before/after table. Load the
example, run it and confirm you get the same result, then complete the **Your
Turn** exercises.

---

## Lab Objectives

| # | Task | Focus | Program files |
|---|------|-------|---------------|
| 1 | Load and Store | Immediate vs direct addressing — `LOD`, `STO` | `W04_TASK1` |
| 2 | Arithmetic and Expressions | ALU arithmetic; splitting an expression into parts | `W04_TASK2` |
| 3 | IF-THEN-ELSE (Check for Zero) | `CMP` sets the flags, `JNZ` chooses a block | `W04_TASK3` |
| 4 | IF-THEN-ELSE (Comparing Two Values) | Direct `CMP B`; `JMP` skips the ELSE block | `W04_TASK4` |
| 5 | Loops (Countdown and WHILE-DO) | Test at the bottom vs test at the top | `W04_TASK5` |
| 6 | Bitwise Logic (NOT and AND) | Logic gates and De Morgan's law | `W04_TASK6` |

---

## Part A — How the CPU Visual Simulator Works

![CPU Visual Simulator](images/figure1.png)

The simulator models a simple **accumulator-based CPU** connected to RAM by three
buses.

| Part | What it does |
|------|--------------|
| **PC** (Program Counter) | Address of the next instruction. Adds 2 after each fetch, because every instruction takes 2 bytes. |
| **IR** (Instruction Register) | The instruction being executed, split into the opcode and the operand. |
| **Control Unit / Decoder** | Decodes the opcode and controls the MUX, the ALU and RAM read/write. |
| **MUX** | Picks the ALU's second input: the IR operand (immediate) or the value read from RAM (direct). |
| **ALU** | Left input is always the ACC; right input comes from the MUX. |
| **ACC** (Accumulator) | The single 16-bit working register. Every load and ALU result ends up here. |
| **SW** (Status Word) | The flags: **Z** = 1 when the last result was zero, **N** = 1 when it was negative. |
| **RAM** | Program and data together; 18 cells at addresses 0 to 34. |
| **Buses** | Address (orange), data (blue) and control (red — read or write). |

### Memory and instruction format

Each address holds 8 bits and an instruction is 16 bits (8-bit opcode + 8-bit
operand), so instructions sit at even addresses and the PC adds 2 after every
fetch. The leftmost opcode bit is the **immediate flag** — `LOD X` is `00000111`
and `LOD #X` is `10000111`.

| Mode | Written as | The operand is … | Meaning |
|------|-----------|------------------|---------|
| Direct | `LOD A` | the address of the value | `[A] -> ACC` |
| Immediate | `LOD #5` | the value itself | `#5 -> ACC` |
| Immediate with a label | `LOD #A` | the address of A, used as a number | loads 34, not A's contents |

- An immediate value must fit in 8 bits: **−128 to 127**. Data values are 16-bit
  (I16): −32768 to 32767.
- `STO` and the jumps only use direct addressing.

### Instruction set (register-transfer notation)

![Instruction set](images/figure2.png)

| Instruction | Direct (`X`) | Immediate (`#X`) | Flags |
|-------------|--------------|------------------|-------|
| `NOP` / `HLT` | no operation / halt | | |
| `JMP X` | `X -> PC` | | |
| `JZ X` / `JNZ X` | jump if Z = 1 / Z = 0 | | reads Z |
| `JN X` / `JNN X` | jump if N = 1 / N = 0 | | reads N |
| `LOD` | `[X] -> ACC` | `#X -> ACC` | |
| `STO X` | `ACC -> [X]` | | |
| `ADD` `SUB` `MUL` `DIV` | `ACC op [X] -> ACC` | `ACC op #X -> ACC` | updates Z, N |
| `AND` | `ACC AND [X] -> ACC` | `ACC AND #X -> ACC` | updates Z, N |
| `CMP` | `ACC - [X]` | `ACC - #X` | updates Z, N only |
| `NOT` | `NOT([X]) -> ACC` | `NOT(#X) -> ACC` | |

> **Important:** jumps test the flags, not the ACC. `LOD` and `STO` leave the
> flags alone, so after loading a value always use `CMP` before a conditional jump.

### Running programs

![Control bar](images/figure3.png)

**Program** runs everything, **Instruction** runs one fetch–decode–execute cycle,
and **Micro step** shows one bus transfer at a time. The **Binary** switch shows
the opcode, immediate flag and operand bits.

### Writing programs

1. Write the pseudo code.
2. Translate it using the IF-THEN-ELSE or WHILE-DO pattern.
3. Comment every line in register-transfer notation, e.g. `; [X] -> ACC`.
4. Run it and compare the before and after states.

A `.cpuvs` file is plain text in the form `LABEL: INSTRUCTION`; everything after
`;` is a comment. Open it with **Load**, save with **Save**.

![IF-THEN-ELSE example code file](images/figure4.png)

---

## Part B — Lab Tasks

The worked examples pad the data block to the bottom of RAM with `NOP` lines so
that the addresses match the figures. The `NOP`s are never executed because the
CPU stops at `HLT`. They are left out of the listings below; the files in the
`W04_TASK` folders contain them.

### Task 1 — Load and Store (Immediate and Direct Addressing)

```
A = 5
ACC = A
```

```asm
        LOD #5      ; #5 -> ACC       (immediate: the number 5)
        STO A       ; ACC -> [A]      (direct: store at address A)
        LOD A       ; [A] -> ACC      (direct: load from address A)
        HLT         ; Halt execution
        ...         ; 13 x NOP
A:      0           ; Data block: A is at address 34
```

| | Before | After |
|---|---|---|
| PC | 0 | 6 (address of HLT) |
| ACC | 0 | 5 |
| Z N | 1 0 | 1 0 (LOD and STO do not update the flags) |
| A (address 34) | 0 | 5 |

![Task 1 after execution](images/figure5.png)

**Your Turn:** explain why the PC counts in 2s; predict `LOD A`, `LOD 34`,
`LOD #34` and `LOD #A`; compare the binary of `LOD #5` and `LOD A`; store 9 in B;
swap two variables.

### Task 2 — Arithmetic and Expressions

**Part 1: `SUM = A + B`**

```asm
        LOD A       ; [A] -> ACC
        ADD B       ; ACC + [B] -> ACC
        STO SUM     ; ACC -> [SUM]
        HLT         ; Halt execution
        ...         ; 11 x NOP
A:      3           ; address 30
B:      4           ; address 32
SUM:    0           ; address 34 (7 after execution)
```

| | Before | After |
|---|---|---|
| PC | 0 | 6 |
| ACC | 0 | 7 |
| Z N | 1 0 | 0 0 |
| SUM (address 34) | 0 | 7 |

![Task 2 after execution](images/figure6.png)

**Part 2: the lecturer's Example 1 — `W = (3 + 4*5) * (2*3 - 1) * 2`**

```asm
        LOD #4      ; Find X = 3+4*5
        MUL #5
        ADD #3
        STO X
        LOD #2      ; Find Y = 2*3-1
        MUL #3
        SUB #1
        STO Y
        MUL X       ; Find X*Y
        MUL #2      ; Find X*Y*2
        STO W       ; Save result to W
        HLT
X:      0
Y:      0
W:      0
```

After execution: X = 23, Y = 5, W = 230, PC = 22.

**Your Turn:** predict `SUB B` and the flags; trace the ACC through Example 1;
calculate Example 2; compute an integer average; explain why `LOD #300` fails.

### Task 3 — IF-THEN-ELSE (Check for Zero)

```
IF (X == 0) THEN FLAG = 1 ELSE FLAG = 0 ENDIF
```

```asm
        LOD X       ; [X] -> ACC
        CMP #0      ; ACC == 0?  (sets Z if yes)
        JNZ NOTZERO ; If not, jump to NOTZERO
ZERO:   LOD #1      ; THEN: #1 -> ACC
        STO FLAG    ;       ACC -> [FLAG]
        JMP END     ;       Jump to END
NOTZERO: LOD #0     ; ELSE: #0 -> ACC
        STO FLAG    ;       ACC -> [FLAG]
END:    HLT         ; Halt execution
        ...         ; 7 x NOP
X:      0           ; address 32 (test value)
FLAG:   0           ; address 34
```

| | Before | After (X = 0) |
|---|---|---|
| PC | 0 | 16 (address of HLT) |
| ACC | 0 | 1 |
| Z N | 1 0 | 1 0 (set by CMP #0) |
| FLAG (address 34) | 0 | 1 |

![Task 3 after execution](images/figure7.png)

**Your Turn:** trace X = 5; rewrite with `JZ`; implement `IF (X < 10)` with `JN`.

### Task 4 — IF-THEN-ELSE (Comparing Two Values)

```
IF (A == B) THEN RESULT = 1 ELSE RESULT = 2 ENDIF
```

```asm
        LOD A       ; [A] -> ACC
        CMP B       ; ACC == [B]?
        JNZ NOTEQ   ; If not, jump to NOTEQ
        LOD #1      ; THEN: #1 -> ACC
        STO RESULT  ;       ACC -> [RESULT]
        JMP END     ;       Jump to END
NOTEQ:  LOD #2      ; ELSE: #2 -> ACC
        STO RESULT  ;       ACC -> [RESULT]
END:    HLT         ; Halt execution
        ...         ; 6 x NOP
A:      4           ; address 30
B:      4           ; address 32 (try changing)
RESULT: 0           ; address 34
```

| | Before | After (A = B = 4) |
|---|---|---|
| PC | 0 | 16 |
| ACC | 0 | 1 |
| Z N | 1 0 | 1 0 (4 − 4 = 0) |
| RESULT (address 34) | 0 | 1 |

![Task 4 after execution](images/figure8.png)

**Your Turn:** change B to 3; remove `JMP END`; trace the lecturer's
`IF_THEN_ELSE_example.cpuvs`; store the larger of A and B in MAX.

### Task 5 — Loops (Countdown and WHILE-DO)

**Part 1: countdown (REPEAT-UNTIL — test at the bottom)**

```asm
        LOD N       ; [N] -> ACC
LOOP:   SUB #1      ; ACC - #1 -> ACC
        STO N       ; ACC -> [N]
        CMP #0      ; ACC == 0?
        JNZ LOOP    ; If not, jump back to LOOP
        HLT         ; Halt execution
        ...         ; 11 x NOP
N:      6           ; address 34 (0 after execution)
```

After execution: N = 0, ACC = 0, PC = 10.

![Task 5 after execution](images/figure9.png)

**Part 2: WHILE-DO (test at the top — the lecturer's `WHILE_DO_example.cpuvs`)**

```
SUM = 0
COUNT = 0
WHILE (COUNT != MAX) DO
    COUNT = COUNT + 1
    SUM = SUM + COUNT
ENDWHILE
```

```asm
        LOD #0      ; 0 -> ACC
        STO SUM     ; 0 -> [SUM]
        STO COUNT   ; 0 -> [COUNT]
WHILE:  LOD COUNT   ; [COUNT] -> ACC
        CMP MAX     ; ACC == [MAX]?
        JZ ENDWHILE ; If yes, jump to ENDWHILE
        ADD #1      ; ACC + 1 -> ACC
        STO COUNT   ; ACC -> [COUNT]   (COUNT++)
        ADD SUM     ; ACC + [SUM] -> ACC
        STO SUM     ; ACC -> [SUM]
        JMP WHILE   ; Jump to WHILE
ENDWHILE: HLT       ; Halt execution
        ...         ; 3 x NOP
MAX:    3
COUNT:  0
SUM:    0
```

After execution: MAX = 3, COUNT = 3, SUM = 6, PC = 22.

**Your Turn:** count the loop passes; fix the N = 0 bug with a WHILE-DO loop;
trace COUNT and SUM; explain why one loop uses `JZ` and the other `JNZ`.

### Task 6 — Bitwise Logic (NOT and AND)

In I16 format, 0 is all zeros and −1 is all ones, so they can be used as FALSE
and TRUE.

```
X = NOT A
Y = NOT B
Z = X AND Y
W = NOT Z
```

```asm
        NOT A       ; NOT([A]) -> ACC
        STO X       ; ACC -> [X]
        NOT B       ; NOT([B]) -> ACC
        STO Y       ; ACC -> [Y]
        LOD X       ; [X] -> ACC
        AND Y       ; ACC AND [Y] -> ACC
        STO Z       ; ACC -> [Z]
        NOT Z       ; NOT([Z]) -> ACC
        STO W       ; ACC -> [W]
        HLT
A:      0
B:      0
X:      -1
Y:      -1
Z:      -1
W:      0
```

**Your Turn:** fill in the truth table; name the gate using De Morgan's law;
write a NAND gate.

### Challenges (optional)

- **C.1** Division without `DIV` — quotient and remainder of 17 / 5.
- **C.2** Factorial of N (N = 5 gives 120).

---

## Review Questions

1. Explain the difference between `LOD 34`, `LOD #34` and `LOD A` (where A is at
   address 34).
2. `CMP` and `SUB` both subtract. What is the difference, and why is `CMP` the
   right choice before a conditional jump?
3. Which register changes when a jump is taken? How does this make loops possible?
4. Which instructions update the Z and N flags? Why do the programs use `CMP`
   straight after `LOD`?
5. An immediate operand is limited to −128 to 127, but a data value can be up to
   32767. Why?

---

## Conclusion

- **Immediate** (`#5`) addressing uses the number itself; **direct** (`A`)
  addressing uses the contents of a memory address.
- Every instruction is 16 bits over two addresses, so the **PC** steps by 2.
- All arithmetic and logic happens in the **accumulator**; `STO` writes results
  back to RAM.
- Jumps test the **Z** and **N** flags, and only ALU instructions update them — so
  `CMP` goes before every conditional jump.
- **IF-THEN-ELSE** needs an unconditional `JMP` to skip the ELSE block; **WHILE-DO**
  tests at the top and jumps back with `JMP`.

---

## Repository Contents

| Path | Description |
|------|-------------|
| `index.html` | GitHub Pages viewer that renders the lab handout PDF |
| `W04_LAB_CPU_Visual_Simulator_Assembly.pdf` | The lab handout |
| `W04 LAB CPU Visual Simulator Assembly.docx` | Editable Word version of the handout |
| `W04_TASK1` … `W04_TASK6` | Worked example programs (`.cpuvs`) — open with **Load** in the simulator |
| `Answer_Key/` | Answer key (Word + PDF) and model solution programs in `Answer_Key/solutions/` |
| `images/` | Figures used in this README |
