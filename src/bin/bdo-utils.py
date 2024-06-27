#!/usr/bin/python3

import os
from re import search
import sys, subprocess, pathlib, pprint, platform


class FatalError(RuntimeError):
    def __init__(self, message):
        super(FatalError, self).__init__(message)


def rip(message, exit_code=-1):
    print("\n\n[FATAL] {0}\n\n".format(message))
    sys.exit(exit_code)


def loge(message):
    print("[ERROR] {0}".format(message))


def logw(message):
    print("[WARN]  {0}".format(message))


def logi(message):
    print("[INFO]  {0}".format(message))


def run_cmd(args, capture_output=False, check=True, verbose=False):
    if verbose:
        print(" ".join(args))
    r = subprocess.run(args, check=check, capture_output=capture_output)
    if capture_output:
        return r.stdout.decode("utf-8").strip()
    else:
        return r.returncode


def compare_file_timestamp(path, latest, chosen):
    if not path.exists():
        return latest, chosen
    timestamp = path.stat().st_mtime
    # print(f"{path} : {timestamp}, {timestamp} < {latest} = {timestamp < latest}")
    if timestamp < latest:
        return latest, chosen
    return timestamp, path


def get_root_folder():
    return pathlib.Path(__file__).resolve().parent.parent.parent.absolute()
