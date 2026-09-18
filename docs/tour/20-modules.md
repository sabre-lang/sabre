---
title: 20. Modules
---

Programs in Sabre can be split into mutiple files called modules. A module is a collection of variables that have been exposed via an `export` declaration.

Local modules can be imported/exported using the following syntax:

```sabre
// Local file "a.sabre"
export let a = "A";
```

```sabre
// Relative file "b.sabre"
import "./a.sabre" as { a };

// Showing the result of our imported module.
Debug.println(a);
```

Import targets are declared with a relative/absolute file-path, or for special imports as file URI's.
