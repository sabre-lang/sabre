---
icon: Map
title: Roadmap
description: A brief outline of the current bug-fixes, goals and other milestones for Sabre
---

### Legend

| Status | Description                    |
| :----: | ------------------------------ |
|   ❌   | Not yet implemented            |
|   🚀   | Currently being developed      |
|   💀   | Has bare-bones implementation  |
|   ⚪   | Finished but needs revising    |
|   ✅   | Finished and ready for release |

## Runtime

| Status | Feature | Description                          | Todo                    |
| :----: | ------- | ------------------------------------ | ----------------------- |
|   💀   | `@...`  | Implement runtime decorators.        | Needs better typing     |
|   💀   | `#...`  | Implement compile-time attributes.   | Overloads not yet done  |
|   ✅   | `use`   | Implement explicit resources.        | -                       |
|   ✅   | `enum`  | Implement enumeration declarations.  | -                       |
|   ⚪   | `class` | Implement class-based inheritance.   | Needs a testing suite   |
|   ⚪   | `match` | Implement `match` statements.        | Needs better typing     |
|   ❌   | `Debug` | Implement debug `breakpoint` method. | Implement a DAP manager |

## Testing

| Status | Suite              | Description                 | Todo |
| :----: | ------------------ | --------------------------- | ---- |
|   ✅   | `builtins/list`    | Testing of `List` builtin   | -    |
|   ✅   | `builtins/number`  | Testing of `Number` builtin | -    |
|   ✅   | `builtins/result`  | Testing of `Result` builtin | -    |
|   ✅   | `builtins/string`  | Testing of `String` builtin | -    |
|   🚀   | `syntax/class`     | Testing of `class` syntax   | -    |
|   🚀   | `syntax/match`     | Testing of `match` syntax   | -    |
|   🚀   | `syntax/use`       | Testing of `use` syntax     | -    |
|   🚀   | `syntax/attribute` | Testing of `#...` syntax    | -    |
|   🚀   | `syntax/decorator` | Testing of `@...` syntax    | -    |

## Libraries

| Status | Library        | Description                     | Todo                           |
| :----: | -------------- | ------------------------------- | ------------------------------ |
|   ✅   | `sabre:assert` | Assertion library methods.      | Additional methods?            |
|   ❌   | `sabre:async`  | Asynchronous utilities.         | -                              |
|   ❌   | `sabre:chrono` | Temporal utilities.             | -                              |
|   ❌   | `sabre:codec`  | Encoding/decoding function.     | -                              |
|   ❌   | `sabre:ffi`    | Native library utilities.       | -                              |
|   ❌   | `sabre:fsys`   | Filesystem utilities.           | -                              |
|   ✅   | `sabre:gc`     | Garbage collection hooks.       | Additional statistics?         |
|   🚀   | `sabre:json`   | JSON/JSONC parsing/serializing. | -                              |
|   ❌   | `sabre:lint`   | Linting plugin bindings.        | -                              |
|   ❌   | `sabre:math`   | Mathematics utilities.          | -                              |
|   ⚪   | `sabre:mem`    | Virtual memory utilities.       | Buffers, allocators, ...       |
|   ❌   | `sabre:path`   | Filesystem path utilities.      | -                              |
|   ❌   | `sabre:rng`    | Random number generators.       | -                              |
|   ⚪   | `sabre:test`   | Testing suite framework.        | Skipping, lifecycle hooks, ... |
|   🚀   | `sabre:toml`   | TOML parsing/serializing.       | -                              |
|   ✅   | `sabre:uuid`   | UUID generators/validators.     | Should there be a validator?   |
|   🚀   | `sabre:yaml`   | YAML parsing/serializing.       | -                              |

## Tooling

| Status | Label     | Description                               | Todo                          |
| :----: | --------- | ----------------------------------------- | ----------------------------- |
|   ✅   | `run`     | Handles starting runtimes.                | See: [Runtime](#runtime)      |
|   ✅   | `test`    | Handles running unit-tests.               | See: [Libraries](#libraries)  |
|   ✅   | `task`    | Executes pre-defined crate tasks.         | Parallel workspaces?          |
|   💀   | `lint`    | Runs extra analysis of scripts/projects.  | Recommended rules, plugins?   |
|   ⚪   | `bundle`  | Packages scripts/projects in executables. | Requires codesigning (Win32)  |
|   ✅   | `format`  | Formats scripts with a builtin styler.    | More configurable options     |
|   ✅   | `serve`   | Starts a language-server instance.        | Code completion is missing    |
|   ✅   | `upgrade` | Upgrades to the latest version of Sabre.  | Needs a release to fully test |

## Documentation

| Status | Section   | Description                          |
| :----: | --------- | ------------------------------------ |
|   ⚪   | Blog      | Still need to implement `feed.xml`   |
|   ⚪   | Crates    | Only stub pages have been made.      |
|   ⚪   | Builtins  | Only stub pages have been made.      |
|   ⚪   | Language  | Fundamentals, advanced concepts, ... |
|   ⚪   | Project   | Contribution points, dashboard, ...  |
|   💀   | Proposals | Only a baseline specification, ...   |
|   ⚪   | Tour      | Some steps are not yet finished.     |
