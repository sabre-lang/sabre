---
title: 12. Casting
---

Sabre does not provide implicit conversions between primitive types. As such, casting to other types must either occur with a conversion function, or by using the cast operator.

```sabre
// Start with an initial numeric value.
let value = 65.4321;

// We can either directly convert the number with the "to_precision" method.
let a: String = value.to_precision();

// Some assignments will naturally do nothing (depends on inferred typing).
let b: Number = value as Number;

// Will otherwise result in compile-time errors.
let c: String = value as String;

// If casting from "Any", this will result in a runtime cast instead of a compile-time one.
let d: Object = value as Any as Object;
```
