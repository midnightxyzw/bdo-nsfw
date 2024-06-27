#!/usr/bin/python3

import sys, pathlib, argparse, timeit
import meta_file, remove_underwear, remove_all_armors
from bin import bdo_utils

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("-i", required=True, help="Path to your BDO PAZ folder")
    ap.add_argument("-o", help="Path to the output folder. Default is the current working directory.")
    ap.add_argument("-m", required=True, help="Select which mod to run. Default is all. Options are: none, remove_underwear, remove_all_armors.")
    args = ap.parse_args()

    # determine output directory
    if not args.o:
        outDir = pathlib.Path.cwd()
    else:
        outDir = pathlib.Path(args.o)

    # determine which mod to run
    run_remove_underwear = False
    run_remove_all_armors = False
    selected_mod = args.m.lower()
    if selected_mod == "remove_underwear":
        run_remove_underwear = True
    elif selected_mod == "remove_all_armors":
        run_remove_all_armors = True
    elif selected_mod == "all":
        run_remove_underwear = True
        run_remove_all_armors = True
    elif selected_mod == "none":
        # in this case, we'll just try read the meta file. this option is mostly for debugging purposes.
        pass
    else:
        bdo_utils.loge("Invalid mod name. Please select from: remove_underwear, remove_all_armors, all")
        sys.exit(1)

    # Read meta file
    meta = meta_file.MetaFile(pathlib.Path(args.i))

    # Generate no_underwear path
    run_times = ""
    if run_remove_underwear:
        t = timeit.timeit(lambda: remove_underwear.remove_underwear(outDir / "_remove_underwear", meta), number=1)
        run_times += f"  remove_underwear: {t:.2f}s\n"
    if run_remove_all_armors:
        t = timeit.timeit(lambda: remove_all_armors.remove_all_armors(outDir / "_remove_all_armors", meta), number=1)
        run_times += f"  remove_all_armors: {t:.2f}s\n"
    bdo_utils.logi(f"Done. Total run times:\n{run_times}")
