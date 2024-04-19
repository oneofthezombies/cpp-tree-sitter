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

## How to Build

```sh
# make build directory
mkdir build && cd build

# generate build configs
cmake .. -G Ninja

# build libcpp_tree_sitter.a and libtree_sitter.a
cmake --build .

# optional) install headers and libraries to the prefix path e.g.) $HOME/.local
sudo cmake --install . --prefix=$HOME/.local

# or install to global e.g.) /usr/local/include, /usr/local/lib
# sudo cmake --install . 
```
