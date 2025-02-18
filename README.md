# Theta
I started this project knowing nothing about C++ and thought it would be fun to learn the language by creating my own programming language, Theta. The language has no real purpose and was built purely for educational purposes.

# Theta Language Specification

## Introduction
Theta is a strongly typed, functional, compiled programming language designed to be data-driven and composable, with built-in support for pattern matching and modular organization through capsules. The language aims to provide a clean and expressive syntax while ensuring type safety and functional programming paradigms.

[Click here to view the formal language grammar in BNF format](doc/theta_grammar.bnf)

---

## Contributing
Thank you for your interest in contributing to the Theta programming language! Here are some guidelines to help you get started:

### Prerequisites

- **Git Submodules**: We use submodules to manage some dependencies. Make sure to initialize and update the submodules by running:
  ```sh
  git submodule update --init --recursive
  ```
- C++ Compiler: Ensure you have a compiler that supports C++17.

### Building Theta

1. **Clone the Repository**: If you haven't already, clone the repository:
  ```sh
  git clone https://github.com/alexdovzhanyn/ThetaLang.git
  cd ThetaLang
  ```
2. **Initialize Submodules**: Initialize and update the submodules for dependencies like Binaryen:
  ```sh
  git submodule update --init --recursive
  ```
3. **Build the Project**: Run the build script to compile Theta:
  ```sh
  ./build.sh
  ```

### Verifying the Installation
To verify that Theta has been installed correctly, run the following command:

```sh
theta --version
```

This should display the current version of Theta.

### Contributing Code

1. **Fork the Repository**: Create a fork of this repository on GitHub.
2. **Create a Branch**: Create a new branch for your feature or bug fix:
  ```sh
  git checkout -b some-feature-branch
  ```
3. **Make Changes**: Implement your changes and commit them to your branch.
4. **Run the Tests**: Run `./build/LexerTest` and `./build/ParserTest` to make sure your changes didn't break any existing functionality
5. **Submit a Pull Request**: Push your changes to your fork and submit a pull request to this repository.

### Testing Changes

Theta has an Interactive Theta (ITH) REPL that can be accessed by just typing `theta` into the terminal. Right now all expressions must fit on
one line, because the REPL expects a newline to mean that you want to submit and compile the code. The REPL doesn't yet interpret the code,
it will just show you the AST that is generated.

To test and run Theta code, you can use the [Theta Browser Playground](https://github.com/alexdovzhanyn/theta-browser-playground) which will
run Theta code in the browser.

### Reporting Issues
If you encounter any issues or have suggestions for improvements, please use the [Issues page](https://github.com/alexdovzhanyn/ThetaLang/issues) to report them. Thank you for contributing to Theta!

## 1. Basic Concepts

### 1.1. Lexical Elements

#### 1.1.1. Identifiers
Identifiers are used to name variables, functions, capsules, and structs. They must start with a letter or underscore and can contain letters, digits, and underscores.

```ebnf
Identifier = Letter (Letter | Digit | "_")*
Letter = "A".."Z" | "a".."z" | "_"
Digit = "0".."9"
```

#### 1.1.2. Keywords
Reserved words that have special meaning in Theta:
```
link, capsule, struct, true, false, void, Number, String, Boolean, Symbol, Enum
```

#### 1.1.3. Comments
Single-line comments start with `//` and extend to the end of the line. Multi-line comments
start with `/-` and extend until reaching a `-/`.

Example:
```theta
// This is a single line comment
// This is another

/-
  This is a multiline comment.
  It extends over multiple lines.
-/
```

---

## 2. Types

### 2.1. Primitive Types
- `String`: Represented by single quotes `'example'`.
- `Boolean`: Represented by `true` or `false`.
- `Number`: Represents both integers and floating-point numbers.
- `List`: Represented by square brackets `[ ]`.
- `Dict`: Represented by curly braces `{ }`.
- `Symbol`: Represented by a colon followed by an identifier, e.g., `:symbol`.

### 2.2. Structs
User-defined data structures composed of primitives or other structs. Structs can only be defined within capsules.
Structs can be referenced by name within the capsule that they are defined, but must be prefixed by their
containing capsule if used in another capsule

```theta
struct StructName {
  fieldName<Type>
  ...
}
```

Example:
```theta
capsule Messaging {
  struct MessageRequest {
    hostname<String>
    port<Number>
    path<String>
    method<String>
    headers<MessageRequestHeaders>
  }
}

// If referenced in another capsule
Messaging.MessageRequest
```

### 2.3. Enums
Enumerated types with custom values represented as symbols. Enum names must be in Pascal case. Enums
are scoped the same as variables, therefore an enum defined in a capsule will be accessible from outside the capsule,
while an enum defined within a function will be scoped to that function.

```theta
enum EnumName {
  :ENUM_1
  :ENUM_2
  ...
}
```

Within a capsule:
```theta
capsule Networking {
  enum Status {
    :SUCCESS
    :FAILURE
    :PENDING
  }
}


// Used like so:
myVar == Networking.Status.SUCCESS

// Or like so, if being referenced from within the capsule:
myVar == Status.SUCCESS
```

Within a function:
```theta
capsule Networking {
  isNetworkRequestSuccess<Boolean> = request<NetworkRequest> -> {
    enum PassingStatuses {
      :SUCCESS
      :REDIRECT
    }

    return Enumerable.includes(PassingStatuses, request.status)
  }

  isNetworkRequestFailure<Boolean> = request<NetworkRequest> -> {
    // PassingStatuses is not available in here
  }
}
```

---

## 3. Variables and Constants

### 3.1. Variable Declaration
Variables are declared by their name, suffixed with their type, followed by an equal sign and their value. Variables are immutable.

```theta
variableName<Type> = value
```

Example:
```theta
greeting<String> = 'Hello, World'
```

---

## 4. Functions

### 4.1. Function Definition
Functions are defined as variables pointing to a block. The return type is specified after the function name.

```theta
functionName<ReturnType> = (param1<Type1>, param2<Type2>, ...) -> {
  // function body
}
```

Example:
```theta
add<Number> = (a<Number>, b<Number>) -> a + b
```

### 4.2. Function Composition
Functions can be composed using the `=>` operator, where the value on the left is passed as the first argument to the function on the right.

```theta
value => function
```

Example:
```theta
requestParams => Json.encodeStruct() => Http.request()
```

---

## 5. Capsules

### 5.1. Capsule Definition
Capsules are static groupings of related functions, variables, and structs, providing modularity and namespace management. All code in Theta must be contained within capsules.

```theta
capsule CapsuleName {
  // variable, function, and struct definitions
}
```

Example:
```theta
capsule Math {
  add<Number> = (a<Number>, b<Number>) -> a + b
  subtract<Number> = (a<Number>, b<Number>) -> a - b

  struct Point {
    x<Number>
    y<Number>
  }

  origin<Point> = @Point { x: 0, y: 0 }
}
```

### 5.2. Importing Capsules
Capsules are imported using the `link` keyword.

```theta
link CapsuleName
```

Example:
```theta
link Http
link Json
```

---

## 6. Pattern Matching

Pattern matching allows for intuitive matching of data structures.

```theta
match value {
  pattern1 >> result1
  pattern2 >> result2
  ...
  _ => defaultResult
}
```

Example:
```theta
matchStatus<String> = status<Enum> -> {
  match status {
    :SUCCESS >> 'Operation was successful'
    :FAILURE >> 'Operation failed'
    _ => 'Unknown status'
  }
}
```

---

## 7. Destructuring
Theta supports list, dictionary, and struct destructuring during variable assignment. It is a powerful
way to pattern match values out of variables, based on the shape of the data:

```theta
myList<List<Number>> = [ 1, 2, 3 ]

// a is 1, b is 2, and c is 3. Notice that you don't have to specify the types of
// a, b, and c here. That is because the compiler can infer the types, because it
// knows we are destructuring a List<Number>, so its values must all be of type Number
[ a, b, c ] = list

myDict<Dict<Number>> = { x: 1, y: 2, z: 3 }

// x is 1, y is 2, z is 3
{x, y, z} = dict
```

---

## 8. File Extension
Theta code files are saved with the extension `.th`

---
## 9. Example Program

Putting it all together, here is a complete example using the discussed features:

```theta
// in Math.th
capsule Math {
    struct Point {
        x<Number>
        y<Number>
    }

    distance<Number> = (point1<Point>, point2<Point>) -> {
        // Calculate distance...
    }

    dimensionalDistanceX<Number> = (point1<Point>, point2<Point>) -> {
      { x: point1X } = point1
      { x: point2X } = point2

      return point2X - point1X
    }
}

// in Main.th
link Math

capsule Main {
    import Math

    point1 = @Math.Point { x: 0, y: 0 }
    point2 = @Math.Point { x: 3, y: 4 }

    distance = Math.distance(point1, point2)
}
```

---

## Conclusion

Theta is designed to be a modern, strongly typed, functional programming language that emphasizes modularity through capsules and clarity through its syntax and structure. This specification outlines the core features and syntax of Theta, providing a foundation for further development and refinement.
