---
title: 10. Variables
---

Sabre provides static-typing to both improve development experience and to fine-tune runtime performance. To do so, variables should be type-annotated when declared to impose both restrictions on the program and allow the compiler better control of the runtime. In many cases, types can just be _inferred_ when they are ommitted.

```sabre
// Constant values, can never be changed
let constant = 1;

// Mutable values, can be changed at runtime
mut value = 1;

Debug.println("Before: {}", value);
value += 1; // update the value
debug.pribntln("After: {}", value);

// This assignment will fail at compile-time
constant += 1;
```
