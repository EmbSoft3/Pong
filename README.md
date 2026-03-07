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

## Build

**Pong** can be built using the [makefile](Pong/Make/makefile) file on the repository.

The variable **TOOLCHAIN\_PATH** must be updated with the path of the ARM toolchain.
The variable **INCLUDES\_API\_PATH** must be updated with the
[include path](https://github.com/EmbSoft3/Mk/tree/main/Mk/Includes) of
the **Mk** operating system.

First, issue a **make clean** command then build the target with **make all**.

Currently versions of compiler used are the followings:

* **gcc** arm-none-eabi-gcc (GNU Arm Embedded Toolchain 10.3-2021.10) 10.3.1
  20210824 (release)
* **g++** arm-none-eabi-g++ (GNU Arm Embedded Toolchain 10.3-2021.10) 10.3.1
  20210824 (release)
* **make** GNU Make 4.4.1 Built for Windows32

Please note, that by default, the application is compiled in release mode with
optimizations enabled (**-Ofast**).

## Installation and execution

Copy the built `.elf` file and the [icon](Pong/Storage/mk_pong.bmp) to the `mk/apps/pong` directory on the storage device (SD card or USB flash drive) used by the target board.

Once Mk is running, **Pong** is installed by default. Click on the icon to run the application.

## License

**Copyright (C)** 2024 **RENARD Mathieu**. All rights reserved.

Mk is free software; It is distributed in the hope that it will be useful.
There is NO warranty; not even for MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.

The content of this repository is bound by the [BSD-3-Clause](LICENSE) license.
