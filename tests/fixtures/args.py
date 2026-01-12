import akane
from util import BINARY, ERROR_CODES


@akane.test("args: empty positional")
def empty_arg():
    res = akane.exec_process((BINARY, ""), unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_INVALID_ARGUMENT"], res.returncode)


@akane.test("args: invalid option")
def args_invalid():
    res = akane.exec_process((BINARY, "--invalid-arg"), unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_INVALID_ARGUMENT"], res.returncode)

    res = akane.exec_process((BINARY, "-i"), unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_INVALID_ARGUMENT"], res.returncode)


@akane.test("args: dash file")
def args_dash_file():
    INPUT = "++++++[>+++++[>++<-]<-]>>+++++."  # outputs A
    res = akane.exec((BINARY, "-"), input=INPUT)
    akane.assert_eq("A", res)
