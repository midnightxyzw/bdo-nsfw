#!/usr/bin/python3

import sys, os, argparse, enum, pathlib, shutil, textwrap


class GenderSelection(enum.Enum):
    F = "Female"
    M = "Male"
    B = "Both"

    @staticmethod
    def choices(indent: str = ""):
        return indent + f"\n{indent}".join([f"{e.name}: {e.value}" for e in OutfitType])


class OutfitType(enum.Enum):
    P = "Pearl Shop Outfits"
    F = "Free (non cash shop) outfits and armors"
    A = "All armors and outfits"

    @staticmethod
    def choices(indent: str = ""):
        return indent + f"\n{indent}".join([f"{e.name}: {e.value}" for e in OutfitType])


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


def warning(message):
    print(ColorCode.colorize(f"\n[WARNING] {message}\n", ColorCode.YELLOW))


def check_folder(target_folder: pathlib.Path):
    if target_folder.is_dir():
        return
    rip(f"The specified folder '{target_folder}' does not exist or is not a directory.")


def check_paz_folder(target_folder: pathlib.Path):
    check_folder(target_folder)
    # if not target_folder.name == "PAZ":
    #     warning(f"The specified folder '{target_folder}' is not a PAZ folder.")
    meta_file = target_folder / "pad00000.meta"
    if not meta_file.exists():
        warning(f"{meta_file} not found. Please make sure '{target_folder}' is pointing to a valid BDO PAZ folder.")


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
        return "_free" in name
    else:
        return True


def deploy_mod(source: pathlib.Path, target: pathlib.Path):
    os.makedirs(target.parent, exist_ok=True)
    # First try deploy mod via symbolic link, if failed. Copy the mod instead.
    try:
        os.symlink(source, target)
        print(f"  {source} -> {target} (symbolic link)")
    except OSError as e:
        # fall back to copy the mod, if symbolic link failed
        print(f"  {source} -> {target} (copy)")
        shutil.copytree(source, target)


def copy_file_no_override(source: pathlib.Path, target: pathlib.Path):
    if target.is_dir():
        target = target / source.name
    if target.exists():
        return
    shutil.copy(source, target)


def copy_mod_tools(paz_folder: pathlib.Path):
    print("\nCopying mod tools...")
    source_folder = pathlib.Path(__file__).parent.parent.resolve()
    copy_file_no_override(source_folder / "Meta Injector.exe", paz_folder)
    copy_file_no_override(source_folder / "PartCutGen.exe", paz_folder)


def apply_patch(paz_folder: pathlib.Path, gender: GenderSelection, outfit_type: OutfitType):
    check_folder(paz_folder)

    # collect all available mods
    print("\nCollecting available mods...")
    mod_source_folder = pathlib.Path(__file__).parent
    mods = [
        # List mods that we always install
        "_00_suzu_nude",
        "_00_npc_and_monster",
    ]
    for m in ["_00_remove_all_armors", "_00_remove_underwear"]:
        dir = mod_source_folder / m
        if not dir.is_dir():
            print(f"Warning: Mod '{m}' not found. Skipping...")
            continue
        for entry in dir.iterdir():
            if not entry.is_dir():
                continue
            if not check_gender(entry.name, gender):
                continue
            if not check_outfit_type(entry.name, outfit_type):
                continue
            mods.append(f"{m}/{entry.name}")

    # create the mod destination folder
    mod_target_folder = paz_folder / "files_to_patch/_midnight_xyzw"
    os.makedirs(mod_target_folder, exist_ok=True)

    # Remove any sub folders and symbolic links started with name "_00_"
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


if "__main__" == __name__:

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

    def highlight(text):
        return ColorCode.colorize(text, ColorCode.YELLOW)

    # print a welcome message
    print("\n\Welcome to the Midnight XYZW mod for Black Desert Online!")

    class CustomFormatter(argparse.HelpFormatter):
        def _split_lines(self, text, width):
            if "choices are:" in text:
                return [x.strip(" ") for x in text.splitlines()]
            else:
                return argparse.HelpFormatter._split_lines(self, text, width)

    parser = argparse.ArgumentParser(formatter_class=CustomFormatter, description="Deploy and customize Midnight XYZW mod for BDO.")
    parser.add_argument(
        "target_folder",
        nargs="?",
        help="Specify the PAZ folder of the BDO game that you want to patch. If not specified, the program will use the current working folder.",
    )
    gender_choices = GenderSelection.choices("\t")
    parser.add_argument(
        "-g",
        "--gender",
        dest="gender",
        type=enum_type(GenderSelection),
        help=f"""Specify gender selection of the Patch. Possible choices are:
                {gender_choices}
                If not specified, the program will ask for it interactively.""",
    )
    outfit_choices = OutfitType.choices("\t")
    parser.add_argument(
        "-a",
        "--armor",
        dest="armor",
        type=enum_type(OutfitType),
        help=f"""Specify the type of armor and outfit to apply the mod to. Possible choices are:
                {outfit_choices}
                If not specified, the program will ask for it interactively.""",
    )
    args = parser.parse_args()
    if not args.target_folder:
        # Use current working folder as the target folder, if not specified. This is the case when user double click the script to run.
        args.target_folder = os.getcwd()
    if not args.gender:
        args.gender = prompt(
            f"""\nWhich gender's armor/outfit you would like to remove: {highlight("F")}(emale), {highlight("M")}(ale) or {highlight("B")}(oth)? """,
            GenderSelection,
        )
    if not args.type:
        args.type = prompt(
            f"""\nWhich type of armor/outfit you would like to remove: {highlight("P")}(earl), {highlight("R")}(egular) or {highlight("A")}(ll)? """, OutfitType
        )

    # ask user to verify and confirm the settings and give user a chance to bail out
    print(
        textwrap.dedent(
            f"""
        You are going to patch game with the following settings:

                target folder : {highlight(args.target_folder)} (make sure this is pointing to your game's PAZ folder)
            outfit/armor type : {highlight(args.type.value)}
                       gender : {highlight(args.gender.value)}
        
        If you are sure about the settings, press {highlight("ENTER")} to continue, or {highlight("Ctrl-C")} to stop."""
        )
    )
    input()
    target_folder = pathlib.Path(args.target_folder)
    check_paz_folder(target_folder)
    apply_patch(target_folder, args.gender, args.type)
    copy_mod_tools(target_folder)
    print(
        f"""\nAll done! Please run the {highlight("PartCutGen.exe")} and {highlight("Meta Injector.exe")} in the {highlight("files_to_patch")} folder to apply the patch to the game."""
    )
