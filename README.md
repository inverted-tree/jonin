<div align="center">
    <img width="128" height="128" src="res/ninja-panda.jpg">
    <h1>jōgetsu</h1>
    <p>A lightweight, high-level, scriptable build system that generates Ninja files for your C and C++ projects.</p>
</div>

> [!WARNING]
> This project is currently in the early development phase and not yet ready for production use. Things are constantly changing and further development may break existing functionalities. Please wait for an official alpha release.


# Installation
## Requirements
- **Lua:** This build system is written in Lua. Thus, you need Lua installed on your system. If you don't yet have Lua, take a look at the [getting started section](https://www.lua.org/start.html) on the official website.
- **Ninja:** jōgetsu only generates build files. To be able to actually build the code, you also need the low-level Ninja build system. To install Ninja, follow the steps on the [official website](https://ninja-build.org/).

## Installation
To use the build system, simply clone this repository:
```sh
git clone https://github.com/inverted-tree/jogetsu.git && cd jogetsu
```
It contains a [shell script](/jogetsu.sh) to run the program in the Lua interpreter.

## Usage
Simply running the shell script executes the build system. jōgetsu expects a `build-options.lua` file, which must return a Lua table. This table specifies the build options, which are then used for generating the Ninja files. After these files have been generated, just run `ninja` and your project should be built.
