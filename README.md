# C++ Tree-sitter

Type and memory safety oriented C++ Tree-sitter binding implementation. 

## Features

- Like `Rust`, not all object ownership transfers can be checked at 
_Compile time_. However, using _Copy/Move constructor_ and 
_Copy/Move assignment operator_ in `C++`, I implemented the concepts _Copyable_,
_Non-copyable_, _Movable_ and _Non-movable_. Except for _Movable_, it is checked
at _Compile time_. If you try to access and use a _Movable_ object that has 
already been consumed, a _Runtime assertion_ occurs in the `Debug` mode.
  - For `TSNode`, it is _Owned_ and _Stack allocated_. So, `ts::Node` was 
  implemented as _Copyable_.
  - For `TSTree`, it is _Owned_ and _Heap allocated_. So, `ts::Tree` was 
  implemented as _Non-copyable_ and _Movable_.
  - For `TSLanguage`, it is _Borrowed_ and _Heap allocated_. So, `ts::Language`
  was implemented as _Non-copyable_ and _Non-movable_.
- For type safety, rather than exposing `TSLogger`'s `void* payload` as is, 
`ts::Logger` exposes its implementation as a virtual method.

## For Contributors

This project was developed based on Bazel.  
Please ensure `bazel` command and after calling the `bazel --version` command, 
check whether the version is higher than `7.0.0`.  

You can get IDE support by running the command below.  

```sh
bazel run @hedron_compile_commands//:refresh_all
```

For more information, please refer to the link below.  

https://github.com/hedronvision/bazel-compile-commands-extractor
