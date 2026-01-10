# bfc

Brainfuck interpreter written in C.

## Motivation

I wanted to write a little program in C just to get to know C a bit better and I thought, this would be a really nice project to do so.

> [!WARNING]  
> I honestly believe no one will ever use this interpreter for any production software, but if you do so, use this software with caution! The ways to shoot yourself in the foot with C are endless, and so to speak I have never used a gun before.

## How to build

```bash
git clone https://github.com/zekrotja/bfc .
make
```

## How to use

```
$ ./dist/bfc --help
usage: bfc [options] [INPUT_FILE]

Arguments:
	INPUT_FILE  Source file (reads from stdin if not specified)

Options:
	-b, --buffer-size <BUFFER_SIZE>  Define the size of the program buffer [default: 30000]
	-d, --debug                      Enable debug mode
	-h, --help                       Print help message
```

Run from a file:

```bash
./dist/bfc inputs/hello_world.bf
```

Run from stdin:

```bash
cat inputs/hello_world.bf | ./dist/bfc
```
