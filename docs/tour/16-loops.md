---
title: 16. Loops
---

Sabre provides two available loop statements, `loop` and `for`.

The `loop` statements, iterates indefinitely until an internally scoped exit occurs. Alternatively, an explicit exit-condition can be given in which `loop` acts like a traditional `while` loop.

```sabre
// Prepare a potential condition variable
let condition = Todo();

// Traditional "while" statement syntax using "loop"
loop (condition) { ... }

// If we remove the condition, we instead get an infinite loop
// (unless there is some internal exit-logic)
loop { ... }
```

The `for` statement is instead used to iterate over potentially iterable values.

```sabre
// We could iterate over a range of numbers
for (nn in Number.range(0, 5)) Debug.println(n);

// We can print each character in a string with its index
for (ch, ii in "abc") Debug.println("{}: '{}'".fmt(ii, ch));
```
