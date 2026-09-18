---
title: 18. Panic
---

When an exceptional error occurs, the `panic` statement should be used.

The default behaviour for `panic` statements is to force an immediate exit with failure message.

```sabre
// Panicking from the main-thread will result in an immediate exit
panic;

// A message can also be attached to the exception
panic "Panic failed because: ...";
```

Exceptions that are thrown within a `Future` can be handled using the `.fails(...)` method. If not explicitly caught, an exception will propagate to the parent thread.

```sabre
// Prepare a future that will fail
let future = Future.async(fn => panic "Future failed!");

// By catching before awaiting, the future will not panic
future.fails(fn {}).await();

// Otherwise if we awaited without catching the program will exit
future.await();
```
