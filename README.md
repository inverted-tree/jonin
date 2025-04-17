<div align="center">
    <img width="128" height="128" src="res/ninja-panda.jpg">
    <h1>Jōnin</h1>
    <p>A lightweight, hackable build system that generates Ninja files for your C projects.</p>
</div>

> [!WARNING]
> This project is currently in development. Breaking changes are expected until a beta is released.


# Installation
## Requirements
- **Lua:** This build system is written in Lua. Thus, you need Lua installed on your system. If you don't yet have Lua, take a look at the [getting started section](https://www.lua.org/start.html) on the official website.
- **Ninja:** Jōnin only generates build files. To be able to actually build the code, you also need the low-level Ninja build system. To install Ninja, follow the steps on the [official website](https://ninja-build.org/).

## Installation
To start generating build files, simply clone this repository:
```sh
git clone https://github.com/inverted-tree/jonin.git && cd jonin
```
It contains a [wrapper shell script](/jonin) to properly run the program in the Lua interpreter. To make this wrapper availabe anywhere on your system, export it to your shell config i.e. .bashrc or .zshrc with this command:
```sh
[ -f ~/.bashrc ] && grep -qxF "export PATH=\"$(pwd):\$PATH\"" ~/.bashrc || echo "export PATH=\"$(pwd):\$PATH\"" >> ~/.bashrc
```

## Usage
Calling the [wrapper script](/jonin) executes the build system. Jōnin expects a [build-options.lua](/test/build-options.lua) file, which is used to script the build process. If you use a different file name or multiple configuration scripts in a project, you need to pass them as an argument to jonin:
```sh
jonin test/test-config.lua
```
Defining multiple configuration scripts per project is discuraged. You can build multiple targets from one configuration by simply declating multiple targets. Each call to jonin generates a new build.ninja file for the project, based on the contents of build-options.lua. Ninja can then be independently used to build the code. Alternatively, jonin macros offer a more user-friendly way to do this.

### Macros
Jonin offers macros to automate parts of the build alongside the creation of a build.ninja file. There exist three default macros:
- `build` calls ninja to compile the code.
- `clean` cleans up the compiled code.
- `run` builds and then runs the project.

Custom macros can be defined inside the build-options.lua file with the `Macro` function, which expects a name, a lua function and an optional description. This implies that macros are just lua functions that take no arguments and return no values (these are called nullary funtions) and thus, arbirary lua code can be executed as a macro. The [example config](test/build-options.lua) contains a macro called **hello**, which just prints *Hello from a macro!* to stdout. Macros can be called when running jonin and will be executed in the order they are specified.
```sh
jonin clean run hello
```
will clean up the compiled code, build it again, run it and finally print *Hello from a macro!*

---

**Note:** This project was called Jōgetsu but was renamed to Jōnin on 20th of March 2025 as a better fit. Jōnin "上忍" (as I understand) means *high (ranking) Ninja* who commands the lower ranking ninjas (which resemble the Ninja build system).
