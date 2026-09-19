---
date: 2026-03-04
author: Reuben Roessler
title: Sabre v0.1.0
---

## Hello, Sabre!

Welcome to the v0.1.0 release of Sabre!

Sabre is a strongly typed programming language focusing on developer experience for scalable and performant concurrent applications. It looks something like this:

```sabre
// Functions can be strictly typed, and variables inferred
let multiply = fn (a: Number, b: Number): Number => a + b;

// When initializing variables, they can also be strictly typed
let future: Future[Number] = multiply::async(6, 7);

// Sabre also exposes concurrency through green-threads
Debug.println(future.await());
```

## Why, Sabre?

[In short, curiosity.](/project)

I started this project as a means of learning more about programming, which then slowly evolved over time into a more fully featured language. My goal was a language similar to TypeScript and Dart, but with more extensive tooling to power the development of a rich standard library written in the language itself.

## What's New?

Realistically, this release contains the baseline implementation of Sabre and ships with an initial set of core development tools. These are in no way final, but at least a starting point for the language to improve itself from. As such, a brief overview of the new features are:

- This is the first core release of Sabre.
- This website which contains documentation and more.
- Fully featured development tooling (barebones):
    - Dynamic and concurrent runtime.
    - Builtin testing/benchmarking framework.
    - Modularized project configurations for monorepos.
    - Builtin type-analysis, linting and formatting.
    - Bundler for shipping standalone executable applications.

## Want More...

Check out [the documentation](/language) for Sabre, or view the project on [GitHub](https://github.com/sabre-lang/sabre).
