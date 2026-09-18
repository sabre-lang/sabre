---
title: 17. Match
---

A `match` statement considers a value and compares against several possible matching patterns. After which and appropriate block of code can be executed if matched successfully.

In the simplest form, a `match` statement compares against values of the same type.

```sabre
match ("a") {
    "a": Debug.println("First latin character"),
    "z": Debug.println("Last latin character"),
    *: Debug.println("Some other character"),
}
```

Unlike other languages, cases do not fall-through to one-another. Each `case` expects a following statement. To instead match more than one case for a code-block, we can combine the cases.

```sabre
match ("a") {
    "a", "A": Debug.println("Found letter 'a'"),
    *: Debug.println("Was not the letter 'a'"),
}
```

Or for more specific matching, we can use a function guard.

```sabre
// Prepare a guard for our values
let guard = fn (ch: String) => ch.to_lower() == "a";

match ("a") {
    guard: Debug.println("Found letter 'a'"),
    *: Debug.println("Was not the letter 'a'"),
}
```

Additionally, we can use classes and builtin types as our guards as well.

```sabre
match ("a") {
    String: Debug.println("Value was a string"),
    Number: Debug.println("Value was a number"),
    *: Debug.println("Did not match a valid type"),
}
```

However, some care should be taken with this as `match` statements attempt their matches greedily from top-to-bottom.

```sabre
match (True) {
    Boolean: Debug.println("Value is a boolean"), // Matched first in declaration order
    True: Debug.println("Value is boolean true"), // Ignored when declared after above guard
}
```
