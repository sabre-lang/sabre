---
title: 21. Crates
---

Alongside modules, Sabre exposes internal and external libraries as crates. Crates are declared by constructing a `_crate.jsonc` at the root of a project directory.

Crates should declare a `main` entry-point for their library module, in which this exposes the directory as the `import` target.

Sabre itself hosts two types of crates:

| Type     | URI Example    | Description                |
| -------- | -------------- | -------------------------- |
| Internal | `sabre:<name>` | Sabres exposed libraries.  |
| External | `crate:<name>` | Installed crate libraries. |
