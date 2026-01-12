import akane
from util import BINARY, ERROR_CODES


@akane.test()
def hello_world():
    INPUT = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++."
    res = akane.exec(BINARY, input=INPUT)
    akane.assert_eq("Hello World!\n", res)


@akane.test()
def hello_world_dynamic_buffer():
    INPUT = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++."

    res = akane.exec_process((BINARY, "--buffer-size", "2"), input=INPUT, unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_BUFFER_INDEX_OUT_OF_BOUNDS"], res.returncode)

    res = akane.exec((BINARY, "--buffer-size", "2", "--dynamic-reallocation"), input=INPUT)
    akane.assert_eq("Hello World!\n", res)
