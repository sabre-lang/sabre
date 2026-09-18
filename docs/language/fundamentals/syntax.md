---
title: Syntax
icon: WholeWord
description: The fundamental syntax of the Sabre programming language
---

This document describes the syntax design of the Sabre programming language. Much of this is provisional until a version `1.0.0` can be produced.

The language is designed to be familiar, whilst also streamlining some elements to simplify readability, usability and internal development. For example, by purposefully designing the syntax, we can more efficiently parse programs (eg: types annotated after identifiers), or we can improve developer experience (eg: module file-names before the imports).

### Comments

Line comments start with `//` and end at newlines.

```sabre
// This is a comment.
```

Sabre does not support block-comments, but instead features doc-comments that are denoted by `//!` instead.

```sabre
//! This is a documentation comment.
```

### Numerics

The following numeric literals are supported:

- `12345` (decimal)
- `0.1234` (floating-point)
- `0xBEEF` (hexadecimal)
- `0o755` (octal)
- `0b10101` (binary)
- `5e+10` (scientific, `+`/`-` is optional)

### Strings

Strings are supported as single-line and as blocks.

```sabre
"example"; // single-line literal
```

#### Sequences

Within a string literal, the following escape sequences are recognized:

| Escape | Meaning           |
| ------ | ----------------- |
| `\t`   | Horizontal tab    |
| `\v`   | Vertical tab      |
| `\n`   | Newline character |
| `\r`   | Carriage return   |

#### Interpolation

Sabre does not yet explicitly allow string interpolation. This can however be achieved with format strings:

```sabre
Debug.println("Hello, {}!".fmt("World"));
```

Format strings use the underlying [`fmt`](https://fmt.dev/) library syntax.

### Tokens

The following is the current symbolic tokens/operators list recognized in Sabre (in order from HIGHEST to LOWEST):

| Operator          | Description                                           |
| ----------------- | ----------------------------------------------------- |
| `[]`              | Typed Expressions                                     |
| `.` `?.` `()`     | Field, Optional Chaining, Function Calls              |
| `as`              | Type Conversion Cast                                  |
| `-` `!` `~`       | Negate, Logical Not, Complement, Increment, Decrement |
| `**`              | Exponentiation (Exponent)                             |
| `*` `/` `%`       | Multiply, Divide, Modulo (Factor)                     |
| `+` `-`           | Addition, Subtraction (Term)                          |
| `<<` `>>`         | Left Shift, Right Shift                               |
| `<` `>` `<=` `>=` | Comparison Operators                                  |
| `is`              | Type Comparson Test                                   |
| `==` `!=`         | Equality Operators                                    |
| `&`               | Bitwise AND                                           |
| `^`               | Bitwise XOR                                           |
| `\|`              | Bitwise OR                                            |
| `&&`              | Logical AND                                           |
| `\|\|` `?:`       | Logical OR, Nullish Coalescing                        |
| `=` `?/:`         | Assignment, Ternary                                   |
| `,`               | Comma Operator                                        |

### Identifiers

Naming rules are similar to other programming languages. They may start with a valid letter, underscore or dollar sign and may contain letters, digits and underscores. Their casing is sensitive.

Some keywords cannot be used as identifiers and as such are considered reserved. These are:

```sabre
let mut
in is as
import export
class public
protected private
fn return panic
type enum namespace
if else match
for loop break continue
```

#### Conventions

For idiomatic Sabre code, these conventions are recommended:

| Item              | Convention         |
| ----------------- | ------------------ |
| Types             | `UpperCamelCase`   |
| Enums             | `UpperCamelCase`   |
| Classes           | `UpperCamelCase`   |
| Namespaces        | `UpperCamelCase`   |
| Boolean Literals  | `UpperCamelCase`   |
| Void Type/Literal | `UpperCamelCase`   |
| Keywords          | `lower_snake_case` |
| Variables         | `lower_snake_case` |
| Members           | `lower_snake_case` |
| Self Literal      | `lower_snake_case` |

### Variables

Variables declare a named value that can be used within Sabre code. They are required to denote their immutability, and can optionally contain a type-annotation. If no typing is given, then their type is auto-inferred.

```sabre
let x: Number = 42;     // Immutable, explicitely typed (Number).
mut y = x;              // Mutable, typing-inferred (Number).
let z: Any = y;         // Immutable, dynamicly typed.
```

### Functions

Functions are a core structural unit within Sabre. For example:

```sabre
let add = fn (a: Number, b: Number): Number { return a + b; };
let sub = fn (a, b: Any) => a + b;
```

The first thing to note is that functions are values. As such, they must be explicitly bound to a variable. The reasoning for this is part of how Sabre aims to be [colorless](/language/advanced/colorless) in calling conventions and concurrency.

Breaking apart `add` we can see that:

- `fn` is the keyword to introduce a function.
- The parameters are contained within parenthesis `(` and `)`.
- It is annotated with the return-type `Number`.
- Contains a body block of code. If the return-type was omitted, it is assumed to be `Void`.
- Requires a closing semicolon since it is a value, not a declaration.

Moving to the `sub` declaration we can further see that:

- Parameter `a` has no typing. Thus it inferred as `Any`.
- The code block is instead replaced with an arrow statement.
- The return-typing is auto-inferred from the arrow result as `Any`.

> **Note**: The code-statement used for functions decides the type-inferrence of return values.

### Flow

Control flow consists of the sequence in which _statements_ are executed. They usually define a scope (enclosed by `{` and `}`) and are terminated by a semicolon.

#### Conditionals

The `if` and `else` statements provide conditional execution of blocks. These are constructed similarly to other C-like languages.

```sabre
if (initial_condition) { ... }
else if (other_condition) { ... }
```

#### Loops

There are two available loop statements, `loop` and `for`.

The `loop` statement can be used to construct finite/infinite loops. This is dependent on the condition, if given.

```sabre
loop (condition) { ... }    // similar to a while loop
loop { ... }                // auto-inferred as infinite
```

The `for` statement supports range-based looping over iterables.

```sabre
for (ch, ii in "abc") Debug.println(ii, ch);
```

The utility statements `break` and `continue` can be used for additional control over loop statements.

#### Return

The `return` statement ends the flow of execution within a function, returning execution to the caller. A value can optionally be provided (dependent on the function context).

```sabre
let sign = fn (n: Number): Number {
  if (i > 0) return 1;
  if (i < 0) return -1;
  return 0; // no-sign
}
```

#### Panic

Although usable as an expression, the `panic` operator throws an immediate exception.

#### Matches

A `match` statement mirrors that of the `switch` statement in other languages, however it also allows for additional `case` patterns.

```sabre
let value = "abc";
let guard = (value) => value == "ab";

match (value) {
    "a": Debug.println("Matches exactly 'a'"),
    String: Debug.println("Value is a string"),
    Number: Debug.println("Value is a number"),
    guard: Debug.println("Matched guard function"),
    *: Debug.println("Did not match anything"),
};
```

Unlike other `switch` statements, Sabre' `match` statement allows checking values against their runtime types (eg: `String`), or through a _guard_ function.

### Classes

A `class` in Sabre is a user-defined record type. All fields are referenced by their names and classes are the primary mechanism for users to construct more complex typings. Sabre supports both named - "nominal" and anonymous - "structural", class types. Nominal classes are all distinct, however structural types are equal if they possess the same fields/types.

#### Nominal

A declaration for a nominal class type could be:

```sabre
class Position {
    public let x: Number = 0;
    public let y: Number = 0;
};
```

Where breaking apart `Position` gets us:

- A `class` introducer
- The name of the class (optional)
- A block containing field declarations
- Declarations are defaulted to `private`

#### Constructors

Classes can be annotated with a _limited_ function signature to denote their construction.

```sabre
// Defines a class with constructor arguments.
class Position(a_x?: Number, a_y?: Number) {
    public let x = a_x ?? 0;
    public let y = a_y ?? 0;
};
```

#### Inheritance

For inheritance, we can have a `class` that extends another. This gives us the following declaration:

```sabre
class Base(_: Any) {};
class Derived => Base("...") {};
```

As can also be seen, we can pass the arguments from the child constructor to the parent.

#### Structural

Classes can also inherit from structural types.

```sabre
type Labeled = {
    label: String;
};

// Enforce that these classes must have a label.
class Derived(a_label: String) implements Labeled {
    public let label = a_label;
};

// Prepare an anonymous object with the "Labeled" interface.
let other: Labeled = {
    let label = "other";
};

// Declare a printer to test all incoming items.
let printer = fn (item: Labeled) => Debug.println(item.label);

printer(other);                 // Valid!
printer(Derived("another"));    // Valid!
```

> **Note**: Although structural types are powerful, they are only accessibly within the type-world. As such, they cannot be used with the `as` or `is` operators.

### Attributes

During compile-time, there are some additional properties that a developer may want to declare. To do this, Sabre introduces attributes with the `#[...]` syntax above a declaration.

This allows us to alter certain properties of values. The current attributes available are:

| Attribute    | Explanation                               |
| ------------ | ----------------------------------------- |
| `Class`      | Alters compile-time `class` properties    |
| `Operator`   | Defines operators for `Object` values     |
| `Overload`   | Allows overloading function signatures    |
| `Internal`   | Exposes internal compilation features     |
| `Deprecated` | Emits deprecation messages as diagnostics |

### Decorators

Alternatively to attributes are decorators. These instead are applied at runtime, and although powerful, have a more restricted scoping for outputs.

### Namespaces

Similar to TypeScript, Sabre allows wrapping variables within a parent `namespace`.

```sabre
namespace Validation {
    // Note: To access variables outside of the namespace
    // scoping, they must be declared as an export.
    export let check = fn (value: Any) { ... };
};
```

### Modules

Sabre includes a file-based module system for multi-file programs. Variables can be exposed to other files by declaring them via the `export` keyword.

```sabre
// File: "a.sabre"
export let value = 42;
```

To then access `value` from another file, it can be imported with the `import` keyword.

```sabre
// File: "b.sabre"
import "a.sabre" as { value };
Debug.println("V:", value);
```

Imports can either destructure the incoming exports, or rename them entirely.

```sabre
import "a.sabre" as A;
```

Since `import` paths are internally contained as [URLs](https://en.wikipedia.org/wiki/URL), this gives Sabre the best option for exposing internal and external crates.

#### Schemes

| Scheme    | Explanation           |
| --------- | --------------------- |
| `<empty>` | Defaults to `file`    |
| `file`    | Filesystem import     |
| `sabre`   | Sabre library crates  |
| `dylib`   | Sabre dynamic crates  |
| `crate`   | User installed crates |

#### Crates

To accomodate complex internal/external dependencies, Sabre implements this similarly to other languages by exposing libraries with a `_crate.jsonc` file. For more information, see the [configuration](../development/configuration) guide.
