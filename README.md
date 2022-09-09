# UFO C++ implementation

This is a plain-vanilla C implementation of UFO. Currently it's written using the C11 standard.

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

* Written using Emacs. I won't get into a holy war over editors. The best editor is the one you prefer to use.
* No 3rd party dependencies. I wrote the lexer, parser, and unit test library.
  * I'll eventually add 3rd party libraries when I get to stuff like JSON and YAML. Now way I'm writing a YAML parser. (Update: but I did write a JSON parser.)
* Compiled using G++.
* Memory tested using valgrind.
* Running in Ubuntu 20 in VirtualBox on a Mac Mini.

## The future of UFO

I don't kid myself into thinking that there is any sort of future for UFO. This is a toy language that I work on for fun. I don't kid myself into thinking that anyone anywhere would want to use UFO for any reason.
