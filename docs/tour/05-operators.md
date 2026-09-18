---
title: 05. Operators
---

The operators and their precedence in Sabre are similar to that of other [C-like languages](https://en.wikipedia.org/wiki/Operator_precedence#Programming_languages).

```sabre
// Numeric operations
Debug.println("1 + 2 = {}", 1 + 2);
Debug.println("1 - 2 = {}", 1 - 2);

// Scientific notation
Debug.println("1e4 is {}, -2.5e-3 is {}", 1e4, -2.5e-3);

// Short-circuiting boolean logic
Debug.println("NOT True is {}", !True);
Debug.println("True AND False is {}", True && False);
Debug.println("True OR False is {}", True || False);

// Bitwise operations
Debug.println("1 << 5 is {}", 1 << 5);
Debug.println("0x80 >> 2 is 0x{:X}", 0x80 >> 2);
Debug.println("0b0011 AND 0b0101 is {:04b}", 0b0011 & 0b0101);
Debug.println("0b0011 OR 0b0101 is {:04b}", 0b0011 | 0b0101);
Debug.println("0b0011 XOR 0b0101 is {:04b}", 0b0011 ^ 0b0101);
```
