---
title: 06. Lists
---

A list is a dynamic collection of values of the same type. Lists can be constructed using brackets `[]`, and has a type signature of `List[T]`. To have a dynamic list of any-typing, the `List` type signature can be used.

```sabre
// Create a list (type signature is superfluous)
let list: List[Number] = [1, 2, 3, 4, 5];

// Indexing starts at "0"
Debug.println("First element of list is: {}", list.get(0));
Debug.println("Last element of list is: {}", list.get(-1));

// Out-of-bounds indexing will cause a panic to occur
// Debug.println("{}", list.get(5));

// `length` returns the total size of the list
Debug.println("Number of elements in list: {}", list.length());

// Can use the safe-access method `get` which returns an optional value.
for (nn in List.range(list.length() + 1)) {
    let value = list.get(nn); // get the current value safely
    if (value.empty()) Debug.println("{}: {}", nn, value.unwrap());
    else if (value.some()) Debug.println("Exceeded list length!");
}
```
