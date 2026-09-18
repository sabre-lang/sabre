---
title: 09. Enums
---

The `enum` keyword defines a set of named constants with an underlying numerical type.

```sabre
enum Season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};
```

By default, the associated values will start at zero and increment by one in definition order. Values can also be explicitly assigned, alongside label assignment as well.

```sabre
enum Severity {
    OKAY("ok"),             // +0
    INFO("info"),           // +1
    WARN("warning") = -1,   // -1
    ERROR("error") = -2     // -2
};
```

Sabre treats enumeration values as numerics, and holds introspection based tools in the constructed enumeration.

```sabre
let severity = Severity.WARN;
Debug.println("Severity: {}", severity); // Will print "-1"
Debug.println("Severity: {}", Enum.name(Severity, severity)); // Will print "WARN"
Debug.println("Severity: {}", Enum.label(Severity, severity)); // Will print "warning"
```
