---
title: Classes
icon: Shapes
description: Defining nominal objects through `class` statements
---

Similar to other object-orientated programming languages, Sabre encapsulates data (fields) in classes to structure reusable code. Classes are the core templates or prototype to objects, which once constructed give a concrete value based on that prototype.

## Declaring Classes

To declare a class in Sabre, they required the following ordered components:

- Starting with the `class` keyword (required)
- Specify a primary constructor (optional)
- Declare a base-class to inherit (optional)
- Implement a selection of interfaces (optional)
- End with a class body (required)

For example:

```sabre
// This is a class that defines a constructor and some public fields
class Person(a_name: String, a_age: Number) {
    public let name = a_name;
    public let age = a_age;
};

// An instance of a 'Person' can be created
let person = Person("John", 42);

// And we can access the properties through dot-syntax
Debug.println(person.name);
Debug.println(person.age);
```

### Constructors

Classes can be optionally declared with a primary constructor. This is similar to a function declaration, minus the return-type (since this is always the class instance).

```sabre
// This is a class with an implicit primary constructor
class Person {};

// This is a class with an explicit primary constructor
class Person(name: String) {};
```

Constructors then pass their arguments onto the enclosing body when the class is instantiated.

### Generics

Since the primary constructor for classes utilizes function syntax for its declaration, generics can be declared in the same fashion.

```sabre
// Declaring a generic class
class Box[T](a_value: T) {
    public let value = a_value;
};
```

## Class Inheritance

To inherit from other classes, a super-type must be placed after the primary constructor and instantiated.

```sabre
// Define a base class instance
class Base(value: Number) {};

// And inherit from it in a derived class
class Derived(value: Number) => Base(value) {};
```

If the base class does not require any constructor arguments, then it can be declared without such:

```sabre
// Define a parameter-less base class
class Base {};

// And inherit without using parameters
class Derived => Base {};
```

## Strict Interfaces

Alongside the nominal subtyping that classes inherit, classes can also utilize structural typing by implemeting given interfaces.

```sabre
// Define a common interface between classes
type Person = {
    name: String;
    occupation: String;
};

// We can then require classes to use the interface
class Student(a_name: String) implements Person {
    public let name = a_name;
    public let occupation = "Student";
};

// Another class specifically implementing the interface
class Adult(a_name: String, a_occupation: String) implements Person {
    public let name = a_name;
    public let occupation = a_occupation;
};

// And we can safely consume these with a polymorphic function
let details = fn (person: Person) {
    Debug.print(person.name, "is a ");
    Debug.println(person.occupation);
};

// Let's see this in action
let child = Student("Billy");
let adult = Adult("Sarah", "Teacher");

details(child);
details(adult);
```
