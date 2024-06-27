#!/usr/bin/python3

import sys, pathlib, termcolor


class FatalError(RuntimeError):
    def __init__(self, message):
        super(FatalError, self).__init__(message)


def rip(message):
    sys.stderr.write(termcolor.colored(f"\n\n[FATAL] {message}\n\n", "red"))
    raise FatalError(message)


def loge(message):
    sys.stderr.write(termcolor.colored(f"[ERROR] {message}\n", "red"))


def logw(message):
    print(termcolor.colored("[WARNING] {0}".format(message), "yellow"))


def logi(message, end="\n", flush=True):
    print(message, end=end, flush=flush)


def get_root_folder():
    return pathlib.Path(__file__).resolve().parent.parent.parent.absolute()


def tryDecodeBinaryString(binaryString):
    try:
        return binaryString.decode("ascii")
    except UnicodeDecodeError:
        try:
            s = binaryString.decode("euc-kr")
            logw(f"Encountered a korean string: {s}.")
            return s
        except UnicodeDecodeError:
            loge(f"Failed to decode binary string: {binaryString}.")
            return None


# Decode binary string. RIP if failed.
def decodeBinaryString(binaryString):
    s = tryDecodeBinaryString(binaryString)
    if s is None:
        rip(f"Failed to decode binary string: {binaryString}.")
    return s
