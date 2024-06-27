import pathlib, shutil, re
import meta_file, remove_underwear
from bin import bdo_utils


def patch_models(outDir: pathlib.Path, meta: meta_file.MetaFile):
    # check if dummy pac file exists
    dummy_pac = pathlib.Path(__file__).parent.parent / "contrib/resorepless-v3.6f/patcher_resources/models/t0072_pumpkin_ground_0001.pac"
    if not dummy_pac.exists():
        bdo_utils.rip("dummy.pac does not exist. Make sure it is in the same folder as this script.")

    patterns = [
        "9_upperbody",
        "10_lowerbody",
        "11_hand",
        # "12_foot", keep boots on to avoid heel floating
        # "13_hel", keep helmets on to avoid missing hair/head
        "14_sho",
        "15_underup",
        "19_cloak",
        "event_costume",
    ]

    # search through model files of all classes
    bdo_utils.logi("Patching models...")
    position = 0
    counter = 0
    total = len(meta.fileBlocks)
    for block in meta.fileBlocks:
        position += 1
        # do not touch Shi class
        if "14_plw" in block.folderName:
            continue
        # folder name needs to contain "1_pc"
        if "1_pc" not in block.folderName:
            continue
        if not any([p in block.folderName for p in patterns]):
            continue
        remove_underwear.generate_patch(outDir, dummy_pac, pathlib.Path(f"{block.folderName}/{block.fileName}"), percent=position * 100.0 / total)
        counter += 1
    bdo_utils.logi(f"\n  {counter} models patched.")

    # Generate .partcutdesc_exclusions.txt to match all patched models
    with open(outDir / ".partcutdesc_exclusions.txt", "w") as f:
        for p in patterns:
            f.write(f"1_pc/*/{p}/\n")


def patch_textures(outDir: pathlib.Path, meta: meta_file.MetaFile):
    underwear_pattern = re.compile(r"_\d{2}_uw_|_99_ub_")
    armor_pattern = re.compile(r"p[a-z]+_(\d{2}|ew)_(ub|lb|hand|sho|underup|cloak)_")

    # search all underwear textures. replace it with a dummy texture
    bdo_utils.logi("Patching underwear textures...")
    counter = 0
    for block in meta.fileBlocks:
        # folder name must contain "character/texture"
        if "character/texture" not in block.folderName:
            continue
        # file name must end with ".dds"
        if not block.fileName.endswith(".dds"):
            continue
        # do not touch SHI class
        if "plw_" in block.fileName:
            continue
        # only care about AO textures
        if not "_ao.dds" in block.fileName:
            continue
        # Ignore files already patched by remove_underwear.py
        if underwear_pattern.search(block.fileName):
            continue
        # Check if it is an armor texture
        if not armor_pattern.search(block.fileName):
            continue
        # patch it with a dummy ao texture
        fullFilePath = block.fullPath()
        remove_underwear.patch_with_dummy_ao_texture(outDir, fullFilePath, block.size)
        counter += 1
    bdo_utils.logi(f"\n  {counter} textures patched.")


def remove_all_armors(outDir: pathlib.Path, meta: meta_file.MetaFile):
    patch_models(outDir, meta)
    patch_textures(outDir, meta)
    # generate a readme file
    with open(outDir / ".README.md", "w") as f:
        f.write(
            """# What's this?

This folder contains patch files to hide all armors, except boots, from all player classes up to release of Scholar.

# How to use it?

- Put it under files_to_patch folder.
- Run PartCutGen.exe to refresh your partcuts_desc.xml
- Run Meta Injector.exe to patch your game.

# Release Notes

- No nude mesh/skin contained here. It is intended to be used with your own
  collection of nude mods and armor patches.

- Generated by Midnight Xyzw's BDO NSFW patch.
"""
        )
