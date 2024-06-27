#!/usr/bin/python3

import sys, pathlib, subprocess, importlib, argparse, platform, concurrent.futures, threading, black
from difflib import unified_diff

utils = importlib.import_module("bdo-utils")


# Filter out unwanted files
def is_our_source(x):
    if x.find("3rdparty") >= 0:
        return False
    if x.find("3rd-party") >= 0:
        return False
    if x.find("contrib") >= 0:
        return False
    return True


def string_diff(str1, str2):
    diff = unified_diff(str1.splitlines(keepends=True), str2.splitlines(keepends=True))
    return "".join(diff)


def format_one_py_file_with_black(x, lock, dry_run=False, quiet=False):
    if not quiet:
        with lock:
            print(f"Black formatting: {x}")

    # Read the contents of the file
    with open(x, "r") as f:
        code = f.read()

    # Format the code using Black
    mode = black.Mode(
        target_versions={black.TargetVersion.PY38},
        line_length=160,
    )
    try:
        formatted_code = black.format_file_contents(code, fast=True, mode=mode)
    except black.NothingChanged:
        # Return false, if the code is not modified (i.e. already formatted)
        return False

    # Print the diff of the changes
    if not quiet:
        with lock:
            d = string_diff(code, formatted_code)
            print(f"{x} violates coding style:\n{d}")

    # Write the formatted code back to the file, if any changes are made and not in dry-run mode
    if not dry_run:
        with open(x, "w") as f:
            f.write(formatted_code)

    # Done
    return True


# function to format one file. Returns True if the file is modified.
def format_one_file(x, lock, clang_format, root_dir, dry_run=False, quiet=False):
    if pathlib.Path(x).suffix == ".py":
        return format_one_py_file_with_black(root_dir / x, lock, dry_run, quiet)
    else:
        # run clang-format
        cmdline = [clang_format, ("--dry-run" if dry_run else "-i"), x]
        if not quiet:
            with lock:
                print(" ".join(cmdline))
        ret = subprocess.run(cmdline, cwd=root_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
        # always print errors to stderr
        err = ret.stderr.decode("utf-8").strip()
        if len(err) > 0:
            with lock:
                sys.stderr.write(err)
        out = ret.stdout.decode("utf-8").strip()
        if not quiet and len(out) > 0:
            with lock:
                print(out)
        return len(out) > 0


def format_all(dry_run=False, diff_only=False, quiet=False):
    # get the root directory of the code base
    root_dir = utils.get_root_folder()
    if not quiet:
        print(f"Root directory: {root_dir}")

    # Gather all GIT managed source files
    patterns = ["*.h", "*.hpp", "*.inl", "*.inc", "*.c", "*.cpp", "*.java", "*.js", "*.glsl", ".hlsl", "*.frag", "*.vert", "*.comp", "*.py"]
    if diff_only:
        cmdline = ["git", "diff", "--name-only", "origin/master", "--"] + patterns
        all_files = subprocess.check_output(cmdline, cwd=root_dir).decode("utf-8").splitlines()
    else:
        cmdline = ["git", "ls-files"] + patterns
        all_files = subprocess.run(cmdline, cwd=root_dir, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout.decode("utf-8").splitlines()

    our_sources = [x for x in all_files if is_our_source(x)]

    # Determine clang-format binary
    system = platform.system()
    if "Windows" == system:
        clang_format = str(root_dir / "src/bin/clang-format-14.0.0.exe")
    elif "Darwin" == system:
        clang_format = str(root_dir / "src/bin/clang-format-apple-darwin-14.0.0")
    else:
        # Assume Linux system. Use clang-format-14 installed by apt
        clang_format = "clang-format-14"

    # create a lock to serialize output
    lock = threading.Lock()

    # run clang-format on all files in parallel
    # for x in our_sources: format_one_file(x)
    modified_counter = 0
    with concurrent.futures.ThreadPoolExecutor() as executor:
        futures = {executor.submit(format_one_file, x, lock, clang_format, root_dir, dry_run, quiet): x for x in our_sources}
        concurrent.futures.wait(futures)
        for future in concurrent.futures.as_completed(futures):
            if future.result():
                modified_counter += 1

    if not quiet:
        print(f"Total {len(our_sources)} files processed. {modified_counter} files are modified.")

    return modified_counter


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("-n", action="store_true", help="Dry run. Print format violations to stderr, if any.")
    ap.add_argument("-q", action="store_true", help="Quiet mode. Mute stdout.")
    ap.add_argument("-d", action="store_true", help="Only process files that are different than the master/main branch")
    args = ap.parse_args()
    format_all(args.n, args.d, args.q)
