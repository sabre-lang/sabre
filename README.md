# Sabre Programming Language

<p align="center">
    <a href="#why-sabre">Why Sabre?</a> |
    <a href="#getting-started">Getting Started</a> |
    <a href="#faq">Contributing</a>
</p>

## Why Sabre?

Many dynamic programming languages prioritize performance, syntax and paradigms over other important development needs such as analysis, testability, scalability, portability and readability. Sabre aims to be the best of both worlds with a statically-typed language that targets a dynamic runtime, and has builtin linting, formating, testing, language-server, bundler and more.

Fundamentally Sabre does not reinvent the wheel, but instead aims to excel at working with the user to improve development workflow. Alongside this, Sabre is designed around the following core features:

- Feature a familiar and readable modern syntax
- Performant static-analysis, with additional strict linting support
- A dynamic runtime for immediate execution, and multiple bundling options for distribution
- Rich standard library, focusing on out-of-the-box testability and development scalability
- A gentle learning curve, with reasonable familiarity to TypeScript, Dart, Rust, and Carbon

The language goals in mind to support these features include:

- [Modular C++ Backend](./docs/project/development.mdx)
    - [Monorepo Build System](./cmake/README.md)
    - [LSP Framework](./source/xlsp/README.md)
    - [Testing Framework](./source/xtest/README.md)
    - [Dependency Injection](./source/xinv/README.md)
    - [Concurrency Framework](./source/xsio/README.md)

- [Language Design](./docs/language/index.mdx)
    - [Familiar Modern Syntax](./docs/language/fundamentals/syntax.md)
    - [Nominal/Structural Types](./docs/language/advanced/type-system.md)
    - [Monadic Error Handling](./docs/language/advanced/exceptions.md)
    - [Fatal Runtime Exceptions](./docs/language/advanced/exceptions.md)
    - [Colorless Function Design](./docs/language/advanced/concurrency.mdx)
    - [Runtime Decorators](./docs/language/fundamentals/decorators.md)
    - [Compile-Time Attributes](./docs/language/fundamentals/attributes.md)
    - [Explicit Resource Management](./docs/language/advanced/resources.mdx)

## Getting Started

Sabre supports Windows (x64), Darwin (x64, Apple Silicon) and Linux (x64, arm64).

```shell
# With the installation script (recommended)
curl -fsSL https://sabre.rroessler.io/install.sh | bash

# Installation script for Windows users (recommended)
powershell -c "irm https://sabre.roessler.io/install.ps1 | iex"
```

## FAQ

**Is there a roadmap for Sabre' development?**

Yes, you can see the [full roadmap](./docs/project/roadmap.md) for more details.

**Can I contribute to this project?**

Not quite yet as Sabre is still an experimental project. Once a version `1.0.0` language can be shipped (or earlier suitable candidate), contribution will become available. Sabre can be [built natively](./docs/project/development.mdx) for those that want to get their hands dirty.
