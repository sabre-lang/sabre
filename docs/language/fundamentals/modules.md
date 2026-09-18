---
title: Modules
icon: FileCodeCorner
description: Code organization that feels intuitive and prevents conflicts
---

During development, it is commonly necessary to structure code into separate source files to modularize functionality, prevent naming collisions and simplify organization.

## Exporting Variables

By default, all variables declared within a source-file are considered to be private. To expose a module-scoped variable, the `export` modifier can be used.

```sabre tab="module.sabre"
// This value is available within this module and externally (see: "script.sabre")
export let value = 42;
```

```sabre tab="script.sabre"
// And can safely be imported and consumed
import "module.sabre" as Module;

// The value is now available for use
Debug.println(Module.value);
```

Alongside value exports, entire modules can be exported:

```sabre title="_exports.sabre"
// We can barrel the exports with this module
export "module.sabre";

// Or we can explicitly rename them before exporting
export "module.sabre" as Module;
```

## Importing Modules

Similar to other languages with import based modules, the `import` keyword can be used to expose variables from other sources.

```sabre title="script.sabre"
// We can run another source without imports
import "module.sabre";

// Or we can explicit pull the variables available
import "module.sabre" as { value };

// Or we can rename the incoming module for use
import "module.sabre" as Module;
```

## Namespace Declarations

At the script-level, `namespace` declarations can be used to organize components. As an example, suppose we have a series of validators we would like to encapsulate together. This can be easily coordinating using a namespace.

```sabre
// Define a namespace specifically for validators
namespace Validation {
    // Allow the validation to use an interface
    export type Checker = {
        accepts: fn (value: String) -> Boolean;
    };

    // We can define locally scoped variables
    let m_letters = "...";
    let m_numbers = "...";

    // This validator checks for letters
    export let letters: Checker = {
        accepts = fn (value: String): Boolean { ... }
    };

    // This validator checks for numbers
    export let numbers: Checker = {
        accepts = fn (value: String): Boolean { ... }
    };
};

// And any variables that are exported within a namespace are available
Debug.println(Validation.letters.accepts("..."));
```
