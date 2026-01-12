import akane
from util import BINARY, ERROR_CODES


@akane.test("syntax: unmatched loop")
def open_loop():
    res = akane.exec_process(BINARY, input="++[", unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_UNMATCHED_LOOP"], res.returncode)

    res = akane.exec_process(BINARY, input="++[>[>++<-]--", unchecked=True)
    akane.assert_eq(ERROR_CODES["ERR_UNMATCHED_LOOP"], res.returncode)
