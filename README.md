# Pong

**Pong** is an arcade game developed for the [**Mk**](https://github.com/EmbSoft3/Mk/) operating system. 
**Pong** is a custom recreation of the classic paddle game, with a few adjustments — the player controls 
a paddle and competes against the computer, trying to score by sending the ball past the computer-controlled paddle.

[![pong](Screenshots/screenshot_pong.bmp "pong")](Screenshots/screenshot_pong.bmp)

To run **Pong**, place the executable file (**.elf**) and its logo on **the file system**
at the following address :
[mk/apps/pong/](https://github.com/EmbSoft3/Mk/tree/main/Mk/Storage/mk/apps/pong)

## Controls

| Key | Action |
|-----|--------|
| **↑** | Move right paddle up |
| **↓** | Move right paddle down |
| **R** | Start / Restart the game |
| **Q** | Quit the application |
| **CTRL+A** | Toogle auto-play mode |

## Installation

Build the application (see [Build](#build) below), then copy `pongGameRelease.elf` and its
icon `mk_pong.bmp` to the Mk file system at:

```
mk/apps/pong/
```

This path corresponds to [`Mk/Storage/mk/apps/pong/`](https://github.com/EmbSoft3/Mk/tree/main/Mk/Storage/mk/apps/pong)
in the Mk repository. Once installed, Pong appears in the Mk home screen application list.

## Build

### Requirements

- [GNU Arm Embedded Toolchain 10.3-2021.10](https://developer.arm.com/downloads/-/gnu-rm)
- GNU Make 4.x
- [Mk Includes](https://github.com/EmbSoft3/Mk/tree/main/Mk/Includes)

### Steps

1. Open `Pong/Make/makefile` and set:
   - `TOOLCHAIN_PATH` — path to your ARM toolchain `bin/` directory
   - `INCLUDES_API_PATH` — path to the Mk [`Includes/`](https://github.com/EmbSoft3/Mk/tree/main/Mk/Includes) directory

2. Build:

```
make clean
make all
```

This produces `pongGameRelease.elf`, ready to install on the target.

> Use the `Debug` target for a `-O0` build with full debug symbols:
> ```
> make Debug
> ```

The application is compiled as a position-independent shared object (`-fPIC -shared`) and is relocatable into any 64 KB SDRAM page by the Mk dynamic loader.

### Compiler versions

| Tool | Version |
|------|---------|
| `arm-none-eabi-gcc` | 10.3.1 20210824 (GNU Arm Embedded Toolchain 10.3-2021.10) |
| `arm-none-eabi-g++` | 10.3.1 20210824 (GNU Arm Embedded Toolchain 10.3-2021.10) |
| `make` | GNU Make 4.4.1 (Windows32) |

---

## License

**Copyright (C)** 2024 **RENARD Mathieu**. All rights reserved.

Mk is free software; It is distributed in the hope that it will be useful.
There is NO warranty; not even for MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.

The content of this repository is bound by the [BSD-3-Clause](LICENSE) license.
