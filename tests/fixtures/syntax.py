import akane
from util import BINARY, ERROR_CODES


@akane.test("syntax: unmatched loop open")
def unmatched_loop_open():
    res = akane.exec_process(BINARY, input="++[", unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_UNMATCHED_LOOP"], res.returncode)

    res = akane.exec_process(BINARY, input="++[>[>++<-]--", unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_UNMATCHED_LOOP"], res.returncode)


@akane.test("syntax: unmatched loop close")
def unmatched_loop_close():
    res = akane.exec_process(BINARY, input="++]", unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_UNMATCHED_LOOP"], res.returncode)

    res = akane.exec_process(BINARY, input="++[>++[>++<-]<-]]", unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_UNMATCHED_LOOP"], res.returncode)

    res = akane.exec_process(BINARY, input="++[>++<-]]]]]]]]]]]]]]]]]]]]]]]]", unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_UNMATCHED_LOOP"], res.returncode)
