# UFO C++ implementation

This is a C++ implementation of UFO.

The target domain of UFO is concurrent, networked, and distributed computing. It's not quite there yet.

## Building the UFO interpreter

```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

## Current status

Goodness, right now it doesn't do **anything**.

## Running example programs

Someday you'll be able to do this.

```
$ bin/ufo
▌▐ ▛▘▛▜ UFO version 5-rc-1
▙▟ ▛ ▙▟ http://github.com/ufo-language
type :? for help

UFO> :l examples/hello.ufo
Hello, World!
UFO> 
```

## Caveat programmator

This version of UFO is not yet compliant with the docs, and probably also diverges a bit from it. I can't decide if I want to change the docs to fit the language, or change the language to fit the docs. I guess they're both sort of converging as I fill out the language.

## Features

* Automatic memory management (mark/sweep GC)

## Development

* Written using Emacs. I won't get into a holy war over editors. The best editor is the one you prefer to use. Yeah, I know modern editors are in some ways "better", but I use Emacs because I like to use Emacs. That means I don't care about min/maxing my keystrokes, or effective committed machine code instructions per millisecond or anything like that. Writing good code is partly science and partly art, and this is how I practice my art.
* Built using CMake.
* Compiled using G++.
* Memory tested using Valgrind.
* Running in Ubuntu 20 in VirtualBox, provisioned using Vagrant, on a Mac Mini.

## The future of UFO

I don't kid myself into thinking that there is any sort of future for UFO. This is a toy language that I work on for fun. I don't kid myself into thinking that anyone anywhere would want to use UFO for any reason.
