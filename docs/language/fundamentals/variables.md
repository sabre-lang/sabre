---
title: Variables
icon: Equal
description: Learn how values and objects are accessed and manipulated in Sabre
---

Variables are symbolically named storage for accessing and manipulating values. They are typically scoped to a region of code with a chosen manner of visibility, and are one of the fundamental building blocks to high-level programming languages.

## Declaring Values

To declare a variable in Sabre, start by using the `let` or the `mut` keyword, followed by the name of the variable. The `let` keyword is used to denote variables that are assigned only once (eg: immutable), and the `mut` keyword is for variables that can be reassigned (eg: mutable).

```sabre
let x: Number = 5;     // Declare an immutable variable named 'x' with a value of '5'
mut y: String = "hi";  // Declare a mutable variable named 'y' with a value of "hi"

x = 42;     // This would result in an assignment exception at compile-time
y = "bye";  // Whilst this is accepted and 'y' will now have a value of "bye"
```

Sabre also supports type inference and automatically resolves the type of a declared variable from its initializer.

```sabre
// The variable 'x' is infered as 'List[Number]'
let x = [0, 1, 2];
```

## Explicit Resources

Alongside the `let` and `mut` keywords, is the `use` declaration which declares a variable as a disposable resource. Such variables are immutable, and only available within their declaring scope (eg: cannot be exported). This is so that upon the closure of their encompassing scope, they are disposed of. For more information, see the [explicit resource management](/language/advanced/resources) section.
