---
date: 2026-06-05
author: Reuben Roessler
title: Progress / May 2026
---

## Reflection

With May behind us and a new release of v0.1.2, I figured it is a good time to reflect on what has been built so far for Sabre. With this release, Sabre now has some astounding functionality which includes the following:

- [Full Featured Dynamic Runtime](/language/toolkit/runtime)
- [Baseline JIT Compilation](/language/advanced/performance)
- [Type-Checking and Extended Linting](/language/advanced/type-system)
- [Modularized Project Configurations](/language/development/configuration)
- [Extensive Integrated Tooling](/language/toolkit/commands)
- [Colorless Functions via Policies](/language/advanced/policies)
- [Concurrency via Green-Threads](/language/advanced/concurrency)
- [Testing/Benchmarking Framework](/language/toolkit/testing)
- [Runtime Decorators](/language/fundamentals/decorators)
- [Compile-Time Attributes](/language/fundamentals/attributes)
- [Explicit Resource Management](/language/advanced/resources)

These are some incredible features, however many of these are still in their early stages of development. For example, the runtime is still fully dependent on the underlying type-checker. Arguably this is necessary for type-safety and correctness, however being able to run a program with the `--typeless` flag can be useful in some contexts. Additionally, there is much work to be done to improve the internal codebase for better language-server outputs and to make the codebase friendlier to contributors.

In reflection, Sabre has some great bones for a new programming language but really needs a substantial facelift to be ready for the long-term.

## Upcoming

Looking ahead, I am currently in the process of rewriting portions of Sabre in my spare time with these lofty goals in mind:

- [x] Improved AST Design &nbsp;&mdash;&nbsp; To facilitate language-server outputs.
- [ ] Better Formatter Support &nbsp;&mdash;&nbsp; To fix quirks and add more options.
- [x] Rework Website Inputs &nbsp;&mdash;&nbsp; Some sections are all over the place.
- [x] Integrate Baseline JIT &nbsp;&mdash;&nbsp; Attempt to prioritize JIT over bytecode.
- [x] Decouple Module Compilation &nbsp;&mdash;&nbsp; Breakup compilation to be independent.
- [x] Performant Type-Checking &nbsp;&mdash;&nbsp; Type-checking needs a minor rework for performance.
- [x] Bind Runtime to Linter &nbsp;&mdash;&nbsp; To accept plugins for writing linting rules.
- [ ] ~~Abstract Scheduling &nbsp;&mdash;&nbsp; Necessary for customized execution policies.~~
- [x] Match Statements &nbsp;&mdash;&nbsp; Expand functionality with a `match` statement.
- [x] Update Foundations &nbsp;&mdash;&nbsp; Implement arena-allocators, and rework foundations.
- [ ] Implement Further Testing &nbsp;&mdash;&nbsp; To improve language testing coverage.

Although this will appear to be a large amount of work (and it probably is), these updates will enhance the codebase and give Sabre the foundations it needs to be ready for library support. Over this next period, I plan to give monthly updates on the current progress of these rewrites. So far I have been working on the improved AST and formatter upgrades, after which I will tackle getting the foundations and abstract scheduling ready to then rebuild the runtime fully.

As for a timeline of these upgrades, I really cannot say. My ideal timeline is to have these changes ready by December for a v0.2.0 release, in which I hope to do [Advent of Code](https://adventofcode.com/) with Sabre. Until then, follow along with these monthly blog posts to see what progress is made!
