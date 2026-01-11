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
	-d, --dynamic-reallocation       Enable dynamic reallocation of the program buffer
	-D, --debug                      Enable debug mode
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

## Language Implementation Details

Some rules of the languages seem not to be as clearly defined, so some assumptions were made based on some documentation I found and other implementations of brainfuck.

The buffer for the cells is pre-allocated and has by default a size of 30000, but you can define a custom size via the `--buffer-size` option. When the pointer moves out of the bounds of the buffer, the program will error. The cell size is 8 bit (`[0..255]`). The values will wrap around when over or underflowing. Currently, the implementation has a limitation that loops can only be nested up to a depth of `1000`. Also the whole program code is load into memory, so the size of your program is limited by the amount of available system memory. Also the input of data (`,` syntax) is currently not implemented.

## TODOs and Ideas

- [ ] Implement input (`,` syntax)
- [ ] Optimizations (like `[-]`) _[see [ref](https://shards.info/github/soya-daizu/bfcr/readme)]_
- [x] Dynamic resizing of the cell buffer
