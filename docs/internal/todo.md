## Todo Items

Here is a basic list of tasks that I would like to implement for Sabre. The immediate tasks are required as-soon-as-possible, whilst the others are marked with their necessaity.

- [ ] Need to work out a way to convert `Object::Instance` fields into suitable Serde mapping outputs. Currently keys are `Value::Symbol` which cannot be converted to `std::string`
- [ ] Better formatter support needs to be implemented through more rigorous testing to find more corner cases that could occur in user-code
- [ ] It would be good for completeness to implement the following execution policies:
    - [x] `::call` / Alias for synchronous function calls
    - [ ] `::spawn` / Assigns a custom scheduler context
    - [x] `::defer` / Helper for inline `Operator.dispose`

### Backlog

- [ ] Decorator command-line switch (eg: to disable/enable decorators based on a given profile, which could be used to enable profiling/logging dynamically)
- [ ] Need to investigate a way to both streamline function/machine frame-stack generation so that construction is fast, similar for both and in a small memory footprint as well
- [ ] The current language server implementation has some strange stutters every once in a while. It would be good to document this for addressing these performance issues
- [ ] Running `hyperfine "sabre test"` sometimes leads to hangs/runtime blocking. This needs some investigation to see why this occurs (most likely something to do with the scheduler in `sabre::xsio`)
    - Changed from using `$::Unique::Pointer` to `$::Shared::Pointer` in `XSIO::Virtual::Thread` for tasks. This does help but now the same race condition occurs more infrequently
