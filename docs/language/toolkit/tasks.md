---
icon: List
title: Tasks
description: Run pre-defined crate configuration tasks
---

## Basic Usage

```shell title="Terminal"
sabre task [options...] <tasks>
```

## Glob Patterns

This subcommand uses glob-like patterns for matching tasks. As such, requesting `build:*` would build all items that start with `build:`, such as `build:alpha`, `build:beta` and `build:delta`.

## Execution Options

### `--filter`

filters the available workspace(s) in which to find an execute the following tasks. Useful for applying tasks to specific or multiple workspaces by name/globs.

### `--quiet` or `--verbose`

Disables and enables the diagnostic outputs. This can be useful for removing or including diagnostics when running tasks.

### `-s`, `--seq` or `--sequential`

Runs all the following tasks in sequential order.

```shell title="Terminal"
# The following task request...
sabre task clean lint build

# ...would be similar to this!
sabre task clean && sabre task lint && sabre task build
```

### `-p`, `--par` or `--parallel`

Runs all the following tasks in parallel order.

```shell title="Terminal"
# The following task request...
sabre task --parallel clean lint build

# ...would be similar to this!
sabre task clean & sabre task lint & sabre task build
```

It also entails that we can run tasks sequentially _and_ in parallel like so:

```shell title="Terminal"
sabre task clean -p build:* -s start:*
```
