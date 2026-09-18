---
title: 11. Types
---

Sabre exposes some additional intrinsic types. These are used to improve the type-system and aid in either causing deliberate or removing obselete compile-time errors. These include:

| Type    | Description                                                 |
| ------- | ----------------------------------------------------------- |
| `Any`   | A catch-all type that can contain anything                  |
| `Never` | This represents types that never overlap (eg: empty unions) |

The `type` statement can be used to either alias, or construct new types. These types are primarily used for reducing boilerplate, but can also be used alongside the utility type operators for better annotations.

```sabre
type Nanoseconds = Number;
type Inches = Number;

let ns: Nanoseconds = 5;
let inches: Inches = 2;

// Note: Aliased types do not provide extra type-safety.
Debug.println("{} ns and {} inches", ns, inches);
```
