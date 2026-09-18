---
title: 07. Objects
---

Apart from primitives, the most common type within Sabre is the `Object` type. This refers to any value that contains dictionary/record-like properties. All objects utilize string-based properties only.

Objects can be defined in a variety of ways. These include:

- Object &ndash; Plain dynamic objects.
- Record &ndash; Keyed dynamic objects.
- Interfaces &ndash; Strictly typed objects.
- Classes &ndash; The classic `class` based object.

```sabre
// Dynamic objects can contain any value.
let point: Object = { x = 100, y = 100 };

// Record based objects specific a secondary typing.
let lookup: Record[Number] = { a = 0, b = 1, c = 2 };

// Interface types explicitly state the necessary properties.
type Person = { name: String, age: Number };
let person: Person = { name = "John", age = 23 };
```
