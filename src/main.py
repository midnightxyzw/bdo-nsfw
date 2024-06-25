#!/usr/bin/python3

import sys, argparse, meta_file, remove_underwear

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("-i", required=True, help="Path to your BDO's PAZ folder")
    ap.add_argument("-o", help="Path to the output folder. Default is the current working directory.")
    args = ap.parse_args()
    meta = meta_file.MetaFile(args.i)
    remove_underwear.remove_underwear(args.o, meta)

