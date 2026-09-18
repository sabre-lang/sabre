---
title: 14. Todo
---

Alongside utility types, there is the `Todo` builtin. This functionality allows for marking various items as unimplemented during development. `Todo` types are marked internally as an alias to `Never` and expose additional linting capabilities for developers.

```sabre
// A default initialized function. If called, would panic with a generic "Todo" exception.
let foo = (value: Todo) { return Todo(); };

// Todo's can also expose messages specific to their development.
let bar = fn () => Todo("Function 'bar' is not yet implemented");
```
