## Todo Items

- [x] Constant Load Propagation (eg: add a constant variable extent for local variables with constant values, so they can propogate instead of moving registers)
- [ ] Decorator command-line switch (eg: to disable/enable decorators based on a given profile, which could be used to enable profiling/logging dynamically)
- [x] Improve JIT compilation by removing as many `Glue::*` callbacks as possible. Should also look into exposing the frame pointer to help with this as well (eg: get encoded isolate/interrupts)
- [ ] Need to investigate a way to both streamline function/machine frame-stack generation so that construction is fast, similar for both and in a small memory footprint as well
- [x] Split `Variable::Captures` into the parsing phase instead of the type-checking phase (this will enable typeless compilation whilst keep performance instead of adding additional visitor overhead)
- [ ] Running `hyperfine "sabre test"` sometimes leads to hangs/runtime blocking. This needs some investigation to see why this occurs (most likely something to do with the scheduler in `sabre::xsio`)
    - Changed from using `$::Unique::Pointer` to `$::Shared::Pointer` in `XSIO::Virtual::Thread` for tasks. This does help but now the same race condition occurs more infrequently
- [x] Currently there are issues with class inheritance that means we cannot reliably execute `? is ?` clauses. Specifically when we use `Object` as a guard (results in mismatched failures on runtime types)

## Rewrite Items

- [x] Improved AST Design
    - [x] Enable extended trivia/comments
    - [x] Nicer syntax bounds for exceptions
        - These can be tested with LSP support
- [ ] Better Formatter Support
    - [ ] Need to implement rigorous testing
    - [ ] Edge cases should be explored
- [x] Rework Website Inputs
    - [x] Often missing headers for dynamic pages
    - [x] Need sections almost entirely inside `docs`
        - Now only dynamic `builtins` and `crates` separate
- [x] Integrate Baseline JIT
    - [x] Should try to revise the baseline JIT
    - [x] To implement baseline JIT, use an attribute #[...]
    - [x] Experiment with tail-call interpreter
    - Note: Need to test more inputs for better outputs
- [x] Decouple Module Compilation
    - [x] Need to allow for safe reflows (graph expiry)
    - Note: Since rework there are no issues with using `Refresh::PARTIAL`
- [x] Performant Type-Checking
    - [x] Need some testing for type-system performance
    - Note: Can visually see improvement with `lint` command (~2x speedup)
- [x] Bind Runtime to Linter
    - [x] Expose the runtime to all toolkit features
    - [x] Expose the `sabre:lint` crate for plugins
    - Note: Yet to implement the crate bindings
- [x] Match Statement
    - [x] Parsing `match` statements
    - [x] Formatting `match` statements
    - [x] Typing `match` statements
    - [x] Bytecode `match` statements
    - [x] Compiling `match` statements
- [-] Abstract Scheduling
    - This might need to go on the backlog
    - Complex but can be available through `::spawn`
- [x] Update Foundations
    - Occuring alongside all the other items
