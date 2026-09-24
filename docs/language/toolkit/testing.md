---
title: Testing
icon: FlaskConical
description: Runs and collates all configured test-runners
---

## Basic Usage

```shell title="Terminal"
sabre test [options...]
```

Runs unit-tests using Sabre' builtin test runner. When requested, any files that fulfill the `--pattern` are evaluated and the coinciding `sabre:test` cases will be queued and executed.

For more information in regard to setting up tests, check out the [`sabre:test`](/crates/test) crate.

## Common Options

### `--pattern`

A globbing pattern used to match test file names. By default, `**/*.test.sabre` is used and will find all files in the current-working subdirectories that end with `.test.sabre`.

### `--bailout`

The `--bailout` option can be used to configure the maximum number of test-failures allowed. By default, Sabre will run all tests and report all failures, however for some CI environments, it may be preferable to terminate earlier to reduce CPU usage.

### `--timeout`

Sets the per-test timeout in milliseconds. If a test times out, it will be canceled and marked as a failure.

### `--seed`

Sets the random number generator seed. This will ensure all calls that require randomness produce the same sequence of _random_ behaviour. By setting a manual seed, this also automatically applies the `--randomize` flag.

### `--randomize`

Sets the randomization flag or seed for test ordering. When enabled without a value, tests will be executed in a randomized order. If a seed is instead given, tests will have a reproducible randomized order.

## Benchmark Options

### `--warmup`

When benchmarking is enabled, this option sets the total warmup calls that are not included in the final benchmark statistics. This is useful for testing programs in a stable execution state.

### `--samples`

The `--samples` option can be used to configure the maximum bound of samples to be taken when benchmarking. During the warmup phase, a single iteration of a benchmark will be executed to determine the suitable range of samples to be taken and uses this value as a maximum allowable within the configured timeout frame.

### `--benchmarks`

This flag enables benchmarking to occur. By default, benchmarking is disabled to reduce total testing time.

## Reporting Options

### `--quiet`

When the `--quiet` flag is given, all verbose `text` reporter outputs will be disabled.

### `--outfile`

This specifies the output file path for reporters to divert their output.

### `--reporter` (Experimental)

Selects a test reporter to be used. Currently defaulted to the pretty `text` reporter, and can be one of the following: `json`, `junit`, `tap`, `text` and `xml`.

## Runtime Options

Most declared [runtime options](/language/toolkit/runtime) can be used when running tests.
