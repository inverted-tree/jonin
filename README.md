<div align="center">
    <img width="128" height="128" src="res/ninja-panda.jpg">
    <h1>Jōnin</h1>
    <p>A lightweight, high-level, scriptable build system that generates Ninja files for your C (and soon C++) projects.</p>
</div>

> [!WARNING]
> This project is currently in the early development phase and not yet considered stable. It is only supported on UNIX-like systems.


# Installation
## Requirements
- **Lua:** This build system is written in Lua. Thus, you need Lua installed on your system. If you don't yet have Lua, take a look at the [getting started section](https://www.lua.org/start.html) on the official website.
- **Ninja:** Jōnin only generates build files. To be able to actually build the code, you also need the low-level Ninja build system. To install Ninja, follow the steps on the [official website](https://ninja-build.org/).

## Installation
To start generating build files, simply clone this repository:
```sh
git clone https://github.com/inverted-tree/jonin.git && cd jonin
```
It contains a [wrapper shell script](/jonin) to properly run the program in the Lua interpreter. To make this wrapper availabe anywhere on your system, export it to your shell config i.e. `.bashrc` or `.zshrc` with this command:
```sh
[ -f ~/.bashrc ] && grep -qxF "export PATH=\"$(pwd):\$PATH\"" ~/.bashrc || echo "export PATH=\"$(pwd):\$PATH\"" >> ~/.bashrc
```

## Usage
Calling the [wrapper script](/jonin) executes the build system. Jōnin expects a [build-options.lua](/test/build-options.lua) file, which is used to script the build process. Thus, calling
```lua
require("jonin")
```
is mandatory when scripting the build. There can be multiple configuration scripts in a project, which can be called separately by passing them as an argument to `jonin`:
```sh
jonin ./test/test-config.lua
```
After the Ninja build files have been generated, just run `ninja` and your project should be built.

---

**Note:** This project was called Jōgetsu but was renamed to Jōnin on 20th of March 2025 as a better fit. Jōnin "上忍" (as I understand) means *high (ranking) Ninja* who commands the lower ranking ninjas (which resemble the Ninja build system).
