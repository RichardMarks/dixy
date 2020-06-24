# DIXY

## What is DIXY?

DIXY is a virtual hardware development ecosystem that I have built from the ground up in modern C++17.

## What can you do with DIXY?

That is a very good question. I cannot really answer it. I am not sure if the language is turing complete.
Someone might find that it is or is not. I would love to know myself. 
So I am declaring an open challenge to all interested to build something cool with it and share it.

## Why did you make DIXY?

A few years ago I started the concept in JavaScript as an in-browser proof of concept. 
I wanted to restart the project and make it into something more.

## Why is this called DIXY?

It refers to names of the virtual hardware registers. D[ata], I[instruction], X, Y.

## DIXY is a name of something else!

Yeah, that is possible. I did not search the web before naming it. If you hold any kind of trademark on the name, 
and you have an issue with my use for this non-profit project, please just let me know and I will change the name.

## Okay... what is included here?

DIXY is an all-in-one development platform that is used through a command line interface (CLI)

There is an assembler, linker, disassembler, emulator, and syntax reference built-in.

Soon there will also be a complete tutorial that will be accessible through the cli as well.

## Wait, an Assembler? So I have to learn assembly?

Sort of. This is very specific syntax for DIXY. And it's super easy. Sixteen mnemonics and no complex syntax nor arguments.

## An Emulator? Can I play games for XYZ console on this?

No. This emulator is only for running binary DIXY programs that are created by the DIXY Linker process.

## Can I use this Disassembler to reverse-engineer app and games on my computer?

No. This disassembler takes a binary DIXY program and outputs DIXY Assembly source.

## Okay, How do I get started?

At the moment there are no prebuilt binaries, so you have to compile DIXY yourself.

You need the DIXY source, CMake version 3+, and a C++17 compatible compiler installed on your machine.

Clone this repository to your machine and use CMake to build the `dixy` executable.

Once you have the executable built, you can put it wherever you like and then run it from a terminal/command line.

Follow the instructions on the screen.

## Argh! I could not compile this stupid thing!

Sorry to hear that. Open an issue here on the project's repository and I will try to help you out.

## Okay, I got it compiled, but I don't know what to do now.

Assuming you have a file `myprogram.das` with DIXY Assembly instruction mnemonics in it:

```
$ dixy assemble myprogram.das myprogram.prg
$ dixy link myprogram.prg myprogram.dxy
$ dixy run myprogram.dxy
```

## I have no idea what the DIXY Assembly instruction mnemonics are!

```
$ dixy syntax
```

Will show you the instruction set that you use.

## Emulator

The virtual hardware of the DIXY emulator is intentionally very simple and highly restrictive by design.
 
There are four registers and 4k of available memory.

The data pointer register; D, refers to the current selected memory location.
The instruction pointer register; I, refers to the current instruction being executed in the running program.
The general storage registers X, and Y are used by the instructions to perform arithmetic and copy values to and from memory.
These four registers and the 4k of memory, and standard output and input are  all that is available for you on this platform.

## I have a question!

Cool. Open an issue and ask your question.

## I have a suggestion!

Great! Open an issue and let me know!

## I want to contribute to this project!

That is fantastic! Fork this repository, make your contributions and submit a pull request!

## I actually made something with this!

Congratulations! If you would like to share it, open an issue and link to the .das or .prg file, 
or copy/paste the .das or .prg file content into a code block.

## License

Basically, you can do whatever you want as long as you include the original copyright and license notice in any copy of the software/source.
(c) 2020, Richard Marks - MIT License [See LICENSE.md](LICENSE.md)
