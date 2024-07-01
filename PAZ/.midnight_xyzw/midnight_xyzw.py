#!/usr/bin/python3

import sys, os, argparse, textwrap, enum, pathlib, shutil

class GenderSelection(enum.Enum):
    F = "Female"
    M = "Male"
    B = "Both"

class OutfitType(enum.Enum):
    P = "Pearl Shop Outfits"
    R = "Regular non-cashshop outfits and armors"
    A = "All armor and outfits"

class ColorCode:
    # Reset code
    RESET = "\033[0m"

    # Foreground colors
    RED = "\033[31m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    BLUE = "\033[34m"
    MAGENTA = "\033[35m"
    CYAN = "\033[36m"
    WHITE = "\033[37m"

    # Background colors
    BG_RED = "\033[41m"
    BG_GREEN = "\033[42m"
    BG_YELLOW = "\033[43m"
    BG_BLUE = "\033[44m"
    BG_MAGENTA = "\033[45m"
    BG_CYAN = "\033[46m"
    BG_WHITE = "\033[47m"

    # Text styles
    BRIGHT = "\033[1m"
    DIM = "\033[2m"
    UNDERLINE = "\033[4m"

    @staticmethod
    def colorize(text, color):
        return f"{color}{text}{ColorCode.RESET}"

def rip(message):
    print(ColorCode.colorize(f"\n[FATAL] {message}\n", ColorCode.RED))
    sys.exit(1)

def check_folder(target_folder : pathlib.Path):
    if not target_folder.is_dir():
        rip(f"The specified folder '{target_folder}' does not exist or is not a directory.")

def check_paz_folder(target_folder : pathlib.Path):
    check_folder(target_folder)
    meta_file = target_folder / "pad00000.meta"
    if not meta_file.exists():
        rip(f"{meta_file} not found. Please make sure '{target_folder}' is pointing to a valid BDO PAZ folder.")
    if not target_folder.name == "PAZ":
        rip(f"The specified folder '{target_folder}' is not a PAZ folder.")

def check_gender(name: str, gender: GenderSelection):
    if GenderSelection.F == gender:
        # make sure name contains word "_female"
        return "_female" in name
    elif GenderSelection.M == gender:
        return "_male" in name
    else:
        return True

def check_outfit_type(name: str, outfit_type: OutfitType):
    if OutfitType.P == outfit_type:
        return "_pearl" in name
    elif OutfitType.R == outfit_type:
        return not "_free" in name
    else:
        return True

def deploy_mod(source: pathlib.Path, target: pathlib.Path):
    os.makedirs(target.parent, exist_ok=True)
    # First try deploy mod via symbolic link, if failed. Copy the mod instead.
    try:
        print(f"  {source} -> {target} (symbolic link)")
        os.symlink(source, target)
    except OSError as e:
        rip("Failed to create symbolic link. Please enable \"Developer Mode\" in Windows Settings to allow symlink creation.")

def apply_patch(paz_folder : pathlib.Path, gender : GenderSelection, outfit_type : OutfitType):
    check_folder(paz_folder)

    # collect all available mods
    print("\nCollecting available mods...")
    mod_source_folder = pathlib.Path(__file__).parent
    mods = [
        # List mods that we always install
        "_00_suzu_nude",
        "_00_npc_and_monster"
    ]
    for m in ["_00_remove_all_armors", "_00_remove_underwear"]:
        dir = mod_source_folder / m
        if not dir.is_dir():
            print(f"Warning: Mod '{m}' not found. Skipping...")
            continue
        for entry in dir.iterdir():
            if not entry.is_dir(): continue
            if not check_gender(entry.name, gender): continue
            if not check_outfit_type(entry.name, outfit_type): continue
            mods.append(f"{m}/{entry.name}")

    # create the mod destination folder
    mod_target_folder = paz_folder / "files_to_patch/_midnight_xyzw"
    os.makedirs(mod_target_folder, exist_ok=True)

    # Remove any subfolders and symbolic links started with name "_00_"
    print("\nCleaning up existing mods...")
    for entry in mod_target_folder.iterdir():
        if entry.name.startswith("_00_"):
            if entry.is_symlink():
                os.unlink(entry)
            elif entry.is_dir():
                shutil.rmtree(entry)

    # Create directory symbolic link for each mod
    print("\nDeploying new mods...")
    for m in mods:
        source = mod_source_folder / m
        target = mod_target_folder / m
        deploy_mod(source, target)

    print("\nPatch applied successfully!")

def enum_type(enum_class):
    def parse_enum(value):
        try:
            return enum_class[value.upper()]
        except KeyError:
            raise argparse.ArgumentTypeError(f"Invalid value '{value}'. Allowed values are: {[e.name for e in enum_class]}")
    return parse_enum

def prompt(question, enum_class):
    while True:
        try:
            value = input(question)
            return enum_type(enum_class)(value)
        except argparse.ArgumentTypeError as e:
            print(e)

if "__main__" == __name__:
    # print a welcome message
    print("\n\Welcome to the Midnight XYZW mod for Black Desert Online!")
    parser = argparse.ArgumentParser()
    parser.add_argument("target_folder", nargs="?", help="The PAZ folder of the BDO game that you want to patch. If not specified, the current folder will be used.")
    # parser.add_argument("-c", "--clean", dest="clean", action="store_true", help="Remove the mod from the specified folder. If this option is specified. All other options will be ignored.")
    parser.add_argument("-g", "--gender", dest="gender", type=enum_type(GenderSelection), help="""Specify gender selection of the Patch. Possible choices are:
                        F: Female only;
                        M: Male only;
                        B: Both genders.
                        If not specified, the program will ask for it interactively.""")
    parser.add_argument("-t", "--type", dest="type", type=enum_type(OutfitType), help="""Specify the type of outfits to apply the mod to. Possible choices are:
                        P: Pearl Shop outfits only;
                        R: Regular (non-cashshop) outfits/armors only;
                        A: All outfits and armors.
                        If not specified, the program will ask for it interactively.""")
    args = parser.parse_args()

    if not args.target_folder:
        args.target_folder = os.getcwd()
    # if args.clean:
    #     clear_patch(pathlib.Path(args.target_folder))
    #     sys.exit(0)
    if not args.gender:
        args.gender = prompt("\nWhich gender's armor/output you would like to remove: F(emale), M(ale) or B(oth)? ", GenderSelection)
    if not args.type:
        args.type = prompt("\nWhich type of outfit/armor you would like to remove: P(earl), R(egular) or A(ll)? ", OutfitType)

    # ask user to verify and confirm the settings and give user a chance to bail out
    print(textwrap.dedent(f"""
        You are going to patch game with the following settings:

                target folder : {args.target_folder} (make sure this is pointing to your game's PAZ folder)
            outfit/armor type : {args.type.value}
                       gender : {args.gender.value}
        
        If you are sure about the settings, press ENTER to continue, or Ctrl-C to stoo."""))
    input()
    apply_patch(pathlib.Path(args.target_folder), args.gender, args.type)
