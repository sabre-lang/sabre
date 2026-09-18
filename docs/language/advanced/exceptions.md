---
icon: CircleX
title: Exceptions
description: Modern error handling for recoverable/unrecoverable states
---

Sabre inherently aims to catch [errors at compile-time](/language/toolkit/linting), however this is not always possible for programs working with external inputs. For example, suppose that an input given to a program references a non-existent file. Since this could not be known during compilation, the act of dealing with this failure is called error handling.

There are a couple of ways that Sabre allows developers deal with errors. At the language level, these include the `panic` keyword and the `Todo` object. For runtime handling of exceptions, a `Maybe` or `Result` value should be used.

## Program Panics

When a program encounters an unrecoverable state, the `panic` keyword can be used to throw an exception.

```sabre
// This function checks given scores for some condition.
let check_score = fn (value: Number) {
    if (score > 90) Debug.println("High Score!");
    else if (score > 50) Debug.println("Okay Score!");
    else if (score > 0) Debug.println("Bad Score!");
    else panic "Scores should never be negative";
}

// These scores should pass ...
check(score(80));
check(score(100));

// ... But this score will panic!
check(score(-1));
```

Any exceptions that are thrown this way will cause the parent `Future` to exit. If this is done at the script level (like above), the program will exit with an uncaught exception.

## Catching Exceptions

Since the `panic` statement is used for unrecoverable errors, it is not intended to be caught. However, any functions that are invoked asynchronously as futures expose a `fails` transform to handle exceptions.

```sabre
// Prepare a future that may fail.
let callback = fn (value: Number) => value < ? panic : True;

// We can then manipulate the resulting exception.
callback::async(-1)
    .fails(fn: Boolean => False)
    .then(fn (state: Boolean) => Debug.println(state));
```

## Unimplemented Code

The `Todo` object can be used both to annotate types and code that has not yet been implemented. During compile-time, these annotations will show warnings about their incompleteness, whilst at runtime will instead crash with an exception.

```sabre
// If we have some code that has not yet been finished.
let incomplete_code = fn: Todo {
    Todo("Unimplemented ...");
};

// When it is called, it will panic with an exception.
incomplete_code();
```

## Safe Returns

Instead of panicking with unrecoverable errors, it is recommended in Sabre to instead make use of the `Maybe` and the `Result` types.

### Nullish Values

Using the `Maybe[T]` type, or the `?: T` syntax for variables, tells the compiler that a value could be voidish (or more colloquially nullish). This means that a value could be of type `T` or `Void`.

```sabre
// We could have a function that has an optional argument.
let coalesce = fn (value?: Number): Number => value ?? 0;

// And when invoked with a "Void" value, will be coalesced to a defaulted number.
Debug.println("Preset: {0}".fmt(coalesce()));
Debug.println("Value: {0}".fmt(coalesce(42)));
```

### Result Values

The `Result[T, E]` type can be used to encapsulate return values that may have a value or a typed exception.

```sabre
// Suppose we have an explicit division handler.
let divide = fn (a: Number, b: Number): Result[Number] {
    if (b != 0) return Result.okay(a / b);
    return Result.error("Division by zero");
};

// We will receive a result from the function.
let result = divide(6, 7);

// And this result can then be unwrapped as needed.
Debug.println("State:", result.is_okay());
Debug.println("Value:", result.unwrap_okay());
```
