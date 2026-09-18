---
title: 08. Classes
---

Classes are general-purpose, flexible constructs that are the building blocks of a program's code. They declare various properties and methods that encapsulate a particular structure or interface.

```sabre
// We can define a class-declaration with given arguments.
class Person(a_name: String, a_age: Number) {
    //  PROPERTIES  //

    private let m_age: Number = a_age;
    private let m_name: String = a_name;

    //  PUBLIC METHODS  //

    public let age = fn: Number => self.m_age;
    public let name = fn: String => self.m_name;
};

// And then instantiate a instance of the class as such.
let person = Person("Sabre", 25);
```

All classes in Sabre have a common super-class of `Object`. This is meant to describe classes as being objects that may exhibit inheritance.

```sabre
class A {};         // Implicitly inherits from `Object`.
class B => A {};    // Explicitly inherits from `A`.
```

To declare an explicit super-class, we place it after the constructor declaration and then instantiate it using the super-constructor.

```sabre
class Base(ii: Number) {};
class Derived(ii: Number) => Base(ii) {};
```
