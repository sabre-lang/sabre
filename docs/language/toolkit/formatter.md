---
icon: Brush
title: Formatter
description: Ensure a consistent coding style across projects
---

## Basic Usage

```shell title="Terminal"
sabre format [options...] [targets...]
```

The builtin formatter handles auto-formatting any given Sabre scripts.

## Common Options

### `--write`

Overwrites all input targets with their formatted results. By default, the formatter will print formatting results to the terminal without overriding the original file.

### `--config`

Specifies the input configuration to be used for formatting. This allows overriding the [crate manifest](/language/development/configuration) associated with the given scripts. By default, any script that is formatted will use it's upward parent crate manifest `format` option.

## Configuration Options

The `format` property within crate files allows specifying customizable formatting rules.

### `line.width`

This is the maximum width in characters that a single of code line can reasonable contain. Depending on the circumstances, this width may be exceeded.

### `indent.width`

When spaces are enabled, this denotes the multiplier width that each indentation of spaces requires.

### `indent.spaces`

Enables the use of spaces instead of tabs.

### `newline.final`

Forcibly places a final newline at the end of code.

### `arrow.compress`

Allows compressing lambdas dynamically. If not enabled then `=>` will always force a newline to occur afterwards.

### `commas.trailing`

Prints trailing commans wherever possible in multi-line comma-separated structures.

### `braces.compress`

Enables compressing content within scoped-blocks. If the included content could fit within a singular line, then the encompassing block will.

### `braces.preserve`

Configures how braces are formatted. When enabled, attempts to fit contents of braces into a singular line, otherwise preserves multi-line content between braces when disabled.

### `terminator_insert`

Prints semicolon terminators at the end of statements depending on the parsing context.
