---
icon: Bug
title: Linting
description: Diagnose additional problems and ensure clean, consistent coding practices
---

## Basic Usage

```shell title="Terminal"
# To run analysis only for a script.
sabre lint [options...] [script]

# Run linting before executing scripts.
sabre run --lint [script]
```

The linter enables additional diagnostic checks

## Common Options

### `--quiet`

Disables all verbose linting diagnostics.

### `--hints`

Enables hint-based diagnostics. These includes type-checking and linting diagnostics that are declared with either a `hint` or `info` type.

### `--config` (Unimplemented)

Specifies the input configuration to be used for linting. This allows overriding the [crate manifest](/language/development/configuration) associated with the given scripts. By default, any script that is linted will use it's upward parent crate manifest `lint` option.

## Configuration Options

The `lint` property within crate files allow specifing which diagnostics and their severities to be applied when linting. It includes the following properties:

### `recommended`

Enables or disables the recommended linting rules.

### `rules`

Any available rules can be have their diagnostic severity altered through this property. This allows customizing the resulting rules that are applied and how they are applied. For example:

```jsonc title="_crate.jsonc"
{
    // ...

    "lint": {
        "rules": {
            // setting a linter rule to "none" disables to rule
            "sabre/no-unsafe-assignments": "none",

            // rules can be forcibly set to "error", effectively forcing their compilance
            "sabre/no-unsafe-calls": "error",

            // rules can also be made to just show hints/information within editors or through the "--hints" flag
            "sabre/no-unsafe-returns": "hint",
        },
    },
}
```

### `plugins` (Unimplemented)

Althrough Sabre ships within many linting rules out-of-the-box, this property allows specifying custom linting plugins.
