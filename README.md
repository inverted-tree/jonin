<div align="center">
    <img width="128" height="128" src="res/ninja-panda.jpg">
    <h1>Jōnin</h1>
    <p>A lightweight, high-level, scriptable build system that generates Ninja files for your C (and soon C++) projects.</p>
</div>

> [!WARNING]
> This project is currently in the early development phase and not yet considered stable. This means that following releases might break your build scripts.


# Installation
## Requirements
- **Lua:** This build system is written in Lua. Thus, you need Lua installed on your system. If you don't yet have Lua, take a look at the [getting started section](https://www.lua.org/start.html) on the official website.
- **Ninja:** Jōnin only generates build files. To be able to actually build the code, you also need the low-level Ninja build system. To install Ninja, follow the steps on the [official website](https://ninja-build.org/).

## Installation
To start generating build files, simply clone this repository:
```sh
git clone https://github.com/inverted-tree/jonin.git && cd jonin
```
It contains a [shell script](/jonin.sh) to run the program in the Lua interpreter.

## Usage
Simply running the shell script executes the build system. Jōnin expects a [build-options.lua](https://github.com/inverted-tree/jonin/blob/main/build-options.lua) file, which is used to script the build process. After the Ninja files have been generated, just run `ninja` and your project should be built.

---

**Note:** This project was called Jōgetsu but was renamed to Jōnin on 20th of March 2025 as a better fit. Jōnin "上忍" (as I understand) means *high (ranking) Ninja* who commands the lower ranking ninjas (which resemble the Ninja build system).
