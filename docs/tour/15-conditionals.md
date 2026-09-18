---
title: 15. Conditionals
---

Sabre features simple conditional `if` statements. These allow executing statements based on the result of given expressions.

```sabre
// Prepare a suitable condition to be checked against
let value: Number = 5.5;

// prepare a check for whether our result succeeds
let result = fn (bounds: Number) => Debug.println("{} LT {}", value, bounds);

// Simplest check could be for range
if (value < 10) result(10);

// Otherwise we could chain our conditionals with "else"
if (value < 0) result(0);
else if (value < 5) result (5);
else if (value < 10) result(10);
else result(Number.Constant.INF);

// By using type-guards we can match types as well with inferrence
if (value is String) Debug.println("{} is a string", value);
else if (value is Number) Debug.println("{} is a number", value);
else Debug.println("Could not infer type of {}", value);
```
