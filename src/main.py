#!/usr/bin/python3

import sys, pathlib, argparse, meta_file, remove_underwear, remove_all_armors

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("-i", required=True, help="Path to your BDO's PAZ folder")
    ap.add_argument("-o", help="Path to the output folder. Default is the current working directory.")
    args = ap.parse_args()

    # determine output directory
    if not args.o:
        outDir = pathlib.Path.cwd()
    else:
        outDir = pathlib.Path(args.o)

    # Read meta file
    meta = meta_file.MetaFile(pathlib.Path(args.i))

    # Generate no_underwear path
    # remove_underwear.remove_underwear(outDir / "_no_underwear", meta)
    remove_all_armors.remove_all_armors(outDir / "_all_armors_removed", meta)

