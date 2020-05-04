# Fog template for C

This is a template repository for creating games with
[Fog](https://github.com/FredTheDino/Fog) and C.

# Compatability
The following combinations have been tested at least once and should "just work":tm:.

Note that there shouldn't be anything stopping you from using something like
MSVC or \*BSD (if you can get SDL2 running) since you have the entire source
available.

| OS      | Compiler                      | Target      |
| ------- | ----------------------------- | ----------- |
| Linux   | gcc (9.3.0)                   | Linux       |
| Linux   | x86\_64-w64-mingw32-gcc       | Windows [1] |
| Windows | gcc (mingw)                   | Windows     |
| macOS   | built-in (clang-1001.0.46.4)  | macOS       |

\[1]: MinGW is not required to run a Windows-executable compiled on Linux.

# Prerequisites

- Python 3 (3.3 and lower not supported)

## Linux and Mac

- A compiler for C and C++
- SDL2, preferably from your package manager. I personally get SDL2 from
  Homebrew (`brew install sdl2`) on macOS and [extra] on Arch (`pacman -S
  sdl2`).

## Windows

- MinGW
- [SDL2 runtime binaries](https://www.libsdl.org/download-2.0.php)

## Targeting Windows from Linux

- MinGW cross compiler, e.g.
  https://aur.archlinux.org/packages/mingw-w64-gcc/
- SDL2 for your cross compiler, e.g.
  https://aur.archlinux.org/packages/mingw-w64-sdl2/

# Usage

1. Press the button labelled "Use this template" above (to the left of "Clone or
   download") to create your own repository with the same directories and files
   as this repository.
2. Clone the repository to your local machine and `cd` into it.
3. Add fog as a submodule.
```shell
$ git submodule add https://github.com/FredTheDino/Fog.git fog
```
4. Compile and run.
```shell
$ make run
```

The repo contains a very basic example which shows how to create some keybinds,
render a point on the screen and start a main-loop.

You don't *have* to add fog as a submodule. However, if you need to fix the
engine during development (or have someone else implement a fix) it's easier to
track and share these changes. Unfortunately Github doesn't add submodules when
creating a copy of a template repository.

## Examples

A few examples (interactive, if you will) are included as branches in this
template. You can either choose "Include all branches" when copying the template
or take a look at them from here. Currently, the following examples exist:

- [Callbacks](https://github.com/sornas/fog-template-c/tree/callback)
- [Audio](https://github.com/sornas/fog-template-c/tree/audio)
- [Audio callbacks](https://github.com/sornas/fog-template-c/tree/audio-callbacks)
- [Input](https://github.com/sornas/fog-template-c/tree/input)

The examples in the documentation exist as well, these are here in order to have
something accessible and easy to play around with.

## Stuff to keep in mind

The built-in engine documentation (`fog/tools/doc.html`) is still
available (also [hosted externally](https://fog.xn--srns-noa9h.se)). Since C
doesn't contain namespaces the names are a bit different but the functions and
parameters stay the same (mostly). For example, `void
Renderer::turn_on_camera(u32 camera_id=0)` becomes `void
fog_renderer_turn_on_camera(u32 camera_id)`.

Do note that particle systems, which used to be classes, now need to be passed
as the first parameter to their methods. `void ParticleSystem::spawn(u32
num_particles=1)` becomes `void fog_renderer_particle_spawn(ParticleSystem
*system, u32 num_particles)` and so on.

Structs need to be a bit more explicit due to differences between C and C++. For
example, you need to explicitly cast on creation.

```c
ParticleSystem system = fog_renderer_create_particle_system(0, 500, fog_V2(0, 0));
system.alive_time = (Span) { 1, 2 };
// ...
fog_renderer_particle_spawn(&system, 10);
```

Another thing to keep in mind is that C doesn't support default parameters so
you need to specify all parameters when calling functions.

If you are unsure about the parameters/usage/name of a function, a quick look in
`inc/fog.h` might refresh your memory.

# Distributing your game

[SDL2 prefers to be linked dynamically](https://hg.libsdl.org/SDL/file/default/docs/README-dynapi.md)
so if you want to distribute your game you need to make sure that the recipient
has access to it. The consensus seems to be that builds targeting Linux
shouldn't need to package SDL2 since it is almost always available in the
package manager, while builds targeting Windows should distribute
SDL2 alongside the executable. For macOS I'd recommend downloading SDL2
separately (either with Homebrew or from https://www.libsdl.org/).

On Windows you also need to distribute pthreads if you don't want to require
MinGW. It can be downloaded from
https://packages.msys2.org/package/mingw-w64-x86_64-libwinpthread. The SDL2
runtime binaries can be downloaded from https://www.libsdl.org/download-2.0.php.
These should go in the same folder as your executable.

If you're compiling on Linux you can use the following commands to create a
distributable .zip-file.

```shell
$ zip game.zip data.fog <executable>  # targeting linux
$ zip -j game.zip data.fog <executable.exe> fog/lib/windows/SDL2.dll fog/lib/windows/winpthreads/libwinpthread-1.dll  # targeting windows
```

The `j`-flag discards the directory structure since the `dll`-files need to be
in the same directory as the executable.

## Licensing of code

A quick word about licensing in case you're distributing your game publicly.

```
Your game
|-- Fog (MIT)
    |-- getline implementation from SO (public domain)
    |-- files generated by glad (probably public domain [2])
    |-- stb_image (public domain)
    |-- SDL2 headers (zlib)
|-- SDL2 (zlib)
|-- (winpthreads) (MIT)
```

\[2]: https://github.com/Dav1dde/glad#whats-the-license-of-glad-generated-code

# Notes

- The Makefile is set to use C99 but both C11 and C18 work fine. You could
  probably use ANSI C as well if you really wanted to.
