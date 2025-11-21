#!/usr/bin/python3

import sys, pathlib, argparse, subprocess
import bdo_utils

ap = argparse.ArgumentParser()
ap.add_argument("input_file", help="Input file. Must be a valid .PAC file.")
ap.add_argument("output_folder", nargs="?", default=".", help="Output folder. Default is the current folder.")
ap.add_argument("-y", "--yes", action="store_true", help="Automatically overwrite existing output file.")
args = ap.parse_args()

input_file = pathlib.Path(args.input_file)
if not input_file.exists():
    bdo_utils.rip(f"{input_file} does not exist.\n")

# Check to ensure the input file is a .PAC file
if input_file.suffix.lower() != ".pac":
    bdo_utils.rip(f"{input_file} must be .PAC file.\n")

output_folder = pathlib.Path(args.output_folder)
if not output_folder.exists() and not output_folder.is_dir():
    bdo_utils.rip(f"{output_folder} does not exist, or is not a directory.\n")

# Check if output file already exists. Ask for confirmation to overwrite.
if not args.yes:
    output_file = output_folder / input_file.with_suffix("").name
    if output_file.exists():
        bdo_utils.rip(f"{output_file} already exists. Do you want to overwrite it? (y/n) ")
        if input() != "y": sys.exit(1)

# look for PACtool.exe
pactool_folder = bdo_utils.get_root_folder() / "contrib/pactool-1.4"
pactool = pactool_folder / "PACtool.exe"
if not pactool.exists():
    bdo_utils.rip(f"{pactool} does not exist.\n")

# get class name from input file name
input_stem = input_file.stem
underbar_index = input_stem.find("_")
if underbar_index < 0:
    bdo_utils.rip(f"Failed to determine class name: {input_file} does not follow the naming convention.\n")
class_name = input_stem[:underbar_index]
bdo_utils.logi(f"Class name: {class_name}")

# Look for the bone file for the class
bone_file = pactool_folder / f"bones/{class_name}_01.pab"
if not bone_file.exists():
    bdo_utils.rip(f"{bone_file} does not exist.\n")
bdo_utils.logi(f"Use bone file: {bone_file}")

# run PACtool.exe to convert the input file to a folder
cmdline = [pactool.absolute(), "-c", input_file.absolute(), bone_file.absolute()]
bdo_utils.logi(' '.join([str(x) for x in cmdline]))
subprocess.run(cmdline, cwd=output_folder)
