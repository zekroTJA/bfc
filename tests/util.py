import os

CURRDIR = os.path.dirname(os.path.realpath(__file__))


def _parse_errorcodes():
    err_codes = {}
    with open(os.path.join(CURRDIR, "..", "src", "errs.h")) as f:
        for line in f.readlines():
            if not line.startswith("#define"):
                continue
            split = line.strip()[len("#define ") :].split(" ", 2)
            if len(split) < 2:
                continue
            err_codes[split[0]] = int(split[1])
    return err_codes


BINARY = os.path.normpath(os.path.join(CURRDIR, "..", "dist", "bfc"))
ERROR_CODES = _parse_errorcodes()
