---
title: Functions
icon: SquareFunction
description: Defining reusable blocks of code that execute procedural steps
---

During development, it is often required to reuse common parts of code. The capability to do so is the through use of functions. In Sabre, functions are both first-class objects, and always declared anonymously, similar to lambdas in other programming languages.

## Declaring Lambdas

To declare a function in Sabre, they require the following ordered components:

- Start with the `fn` keyword (required)
- Specify generics with `[]` (optional)
- Specify parameters with `()` (optional)
- Include a return-typing (optional)
- End with a function body (required)

For example:

```sabre
// This declares a function that has one parameter, 'x' of type 'Number', and a return value also of type 'Number'
let double = fn (x: Number): Number {
    return 2 * x;
};

// Alternatively, functions can also be declared using arrow-syntax to fit neatly on one line
let square = fn (x: Number): Number => x * x;
```

### Generics

Functions can optionally be declared with generic parameters enclosed within brackets.

```sabre
// Sabre will infer the type-parameter 'T' to what is called
let generic = fn [T](item: T): Any => ...;
```

### Parameters

Within function declarations, parameters are declared using the `name: Type` notation. By default, all function parameters are immutable (implicitly declare as `let`), but can be made mutable by explicitly declaring parameters with the `mut` keyword.

```sabre
// Declaring 'value' with the 'mut` keyword allows reassignment within the function scope
let mutate = fn (mut value: Any) => ...;
```

When a function is declared without parameters, it can be written with either of the following syntaxes:

```sabre
// With an explicitly empty set of parameters
let explicit = fn () => ...;

// Or with an implicitly empty set of parameters
let implicit = fn => ...;
```

Parameters can also be made optional, either at the type-level or by assigning a default value.

```sabre
// When called without arguments, this will default 'value' to '0'
let defaulted = fn (value: Number = 0) { ... };

// When called without argumens, this will default 'value' to the 'Maybe' type default of 'Void'
let optional = fn (value?: Number) { ... };
```

### Variadics

Sabre also supports functions with a variadic number of arguments. For these cases, the rest parameter syntax is used and must be placed before the variadic parameter. There can only be one variadic parameter per function, which then passes an array of arguments to the function when called.

```sabre
// This function accumulates a sum of the given numbers
let sum = fn (...items: Number): Number {
    // prepare a summation result
    mut result = 0;

    // iterate over the items given
    for (nn in items) result += nn;

    // return the resulting sum
    return result;
};

// And can be called with a variable number of arguments
Debug.println(sum(1, 2, 3, 4, 5));
```

### Return Types

When declaring a function, the return-type can be contextually dependent on the type of block body that is defined. Unless explicitly specified, the return-type is infered to be `Any` for functions using arrow-syntax, or infered to be `Void` for functions using curly brace blocks.

Sabre currently does not infer return-types based on the block bodies due to the complexity of their flow control, and how their type inference appears unclear both to readers and sometimes to the compiler. As such, using a standardized inference scheme simplifies both these aspects for the reader and the compiler.

```sabre
let foo = fn => ...;    // Always implicitly infered as 'Any'
let bar = fn { ... };   // Always implicitly infered as 'Void'

let baz = fn: Number => 5;            // Explicitly declared as 'Number'
let qux = fn: String { return ""; };  // Explicitly declared as 'String'
```

## Calling Functions

Once a function is created, it can be called using the standard approach:

```sabre
// This invokes the function with the default calling policy
let result = double(5);
```

For generic functions, type-arguments can be specified after the variable name, or omitted entirely so that they can be infered from the context.

```sabre
// Explicitly giving type-arguments to a generic
generic<Number>(42);

// For some generics, type inference will resolve 'T' with type 'Number'
generic(42);
```

Functions can also be called with different conventions, through named "execution policies". This uses the following syntax specific to Sabre:

```sabre
// This will call the function using the 'async' execution policy which returns a 'Future[Number]'
let future = double::async(5);
```

For more information, see the [concurrency](/language/advanced/concurrency) and [execution policy](/language/advanced/policies) segments.

## First-Class Citizens

Since functions are considered to be [first-class citizens](https://en.wikipedia.org/wiki/First-class_citizen), in which it is possible to pass functions as arguments and to also return functions.

```sabre
// Declaring a multiplier factory type and function
type Multiplier = fn (value: Number) -> Number;
let Multiplier = fn (factor: Number): Multiplier {
    // returning a suitable multiplication closure
    return fn (value: Number): Number => factor * value;
};

// Creating specific multiplier functions
let doubler = Multiplier(2);
let tripler = Multiplier(3);

// Calling the resolved closures will result in '10' then '15'
Debug.println(doubler(5));
Debug.println(tripler(5));
```
