import json

import akane
from util import BINARY, ERROR_CODES


@akane.test("general: hello world")
def hello_world():
    INPUT = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++."
    res = akane.exec(BINARY, input=INPUT)
    akane.assert_eq("Hello World!\n", res)


@akane.test("general: dynamic buffer")
def hello_world_dynamic_buffer():
    INPUT = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++."

    res = akane.exec_process((BINARY, "--buffer-size", "2"), input=INPUT, unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_BUFFER_INDEX_OUT_OF_BOUNDS"], res.returncode)

    res = akane.exec((BINARY, "--buffer-size", "2", "--dynamic-reallocation"), input=INPUT)
    akane.assert_eq("Hello World!\n", res)


@akane.test("general: read input")
def read_input():
    res = akane.exec((BINARY, "--input", "a"), input=",.")
    akane.assert_eq("a", res)

    res = akane.exec((BINARY, "--input", "Hello World!"), input=",[.,]")
    akane.assert_eq("Hello World!", res)


@akane.test("general: deny input")
def deny_input():
    res = akane.exec_process((BINARY, "--deny-input"), input="+++++[.-]")
    akane.assert_eq(0, res.returncode)

    res = akane.exec_process((BINARY, "--deny-input"), input=",[.,]", unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_INPUT_DISALLOWED"], res.returncode)


@akane.test("general: hello world")
def zero_byte_json():
    INPUT = "."
    res = akane.exec((BINARY, "--json", "--buffer-size", "1"), input=INPUT)
    res_obj = json.loads(res)
    akane.assert_eq("\u0000", res_obj["output"])
