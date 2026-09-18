---
title: 25. Concurrency
---

Syntactically speaking, all functions within Sabre can be called asynchronously through the `Future.async` static method or more succinctly using an execution policy.

```sabre
// Let's construct a function that wraps values.
let enclose = fn (value) => value;

// This allows us to dispatch asynchronous functions.
mut future = Future.async(enclose, 42);

// Or, we can use an execution policy instead.
future = enclose::async(42);
```

The result of both of these is a `Future[T]` value, that can be awaited on later using the `Future.await` method.

```sabre
// Let's wait for the future...
let result = future.await();

// ...and print the result to the console.
Debug.println("Result {0}".fmt(result));
```

Additionally, futures can be chained and managed through the `.then` and `.fails` methods.

```sabre
// Prepare a future that may fail if it is negative.
let acceptor = fn (value: Number): Boolean => value < 0 ? panic : True;

// We can then manipulate the result with futures.
acceptor::async(-1)
    .fails(fn: Boolean => False)
    .then(fn (positive: Boolean): String => positive ? "Positive" : "Negative")
    .then(Debug.println);
```

See the [concurrency documentation](/language/advanced/concurrency.md) for a more in-depth look.
