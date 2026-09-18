---
icon: AtSign
title: Decorators
description: Runtime annotations to empower the metaprogramming capabilities of Sabre
---

To lay the foundations for prospective reflection capabilities in Sabre, there needed to be a way to support annotations of variables at runtime. Decorators, similar to those in TypeScript, provide a way to add these meta-programming features.

A decorator is a special annotation expressed in the form `@expression`, where the expression must evaluate to a function that when called at runtime receives a targeted declaration for introspection and returns the target in-place.

## Usage

The syntax of a decorator is relatively simple, where we simply prepend the `@` operator before the decorator expression that is then applied to a chosen target.

```sabre
// A basic decorator declaration.
let simple = fn (target): Any {
    Debug.println("Simple decorator called");
    return target; // should always return
};

// Applying the decorator to a target.
@simple
let target = fn {};
```

Unlike other programming languages, decorators can be applied to any valid declaration from below:

- Variables
- Enums
- Classes
- Namespaces

## Factories

Since Sabre treats functions as first-class citizens, this means that decorators can be constructed with an enclosed context.

```sabre
// This is a decorator factory that receives a context.
let color = fn (value: String): Any {
    // This is the decorator that is constructed.
    return fn (target: Any): Any {
        // We can then use both 'target' AND 'value'
    };
};
```

## Evaluation

Decorators are executed only once in sequential order, after the incoming declaration has been initialized. So for the following:

```sabre
// A decorator that logs functions.
let log_eval = fn (label: String): Any {
    Debug.println("Eval: '{0}'".fmt(label));
    return fn (target) => target;
};

@log_eval("A")
let a = 42;

@log_eval("B")
let b = fn {};

@log_eval("C")
class C {
    @log_eval("D")
    let d = fn {};

    @log_eval("E")
    let e = fn {};
};

@log_eval("F")
let f = C();

@log_eval("G")
let g = C();
```

We should see an order similar to:

```shell title="Console"
Eval: 'A'
Eval: 'B'
Eval: 'C'
Eval: 'D'
Eval: 'E'
Eval: 'F'
Eval: 'D'
Eval: 'E'
Eval: 'G'
```

This example is also great to notice that the evaluation order of class fields only occurs when instantiated.
