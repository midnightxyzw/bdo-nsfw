#!/usr/bin/python3

import pathlib, shutil, re
import meta_file
from bin import bdo_utils

# Starter patterns
starter_regex = re.compile(r'''.*(
                           _0[1|2|3]_.*_\d{4} # this is the default pattern for non cash shop armors
                           ).*''', re.VERBOSE)

# female class patterns
female_regex = re.compile(r'''.*(
                          p[a-z]+w_ # most of female classes fall into this pattern
                          |pmyf_ # Woosa's ID is special
                          )''', re.VERBOSE)

def is_female_starter(block: meta_file.FileBlock):
    fullPath = block.fullPath()
    return female_regex.match(fullPath) and starter_regex.match(fullPath)

def is_female_non_starter(block: meta_file.FileBlock):
    fullPath = block.fullPath()
    return female_regex.match(fullPath) and (not starter_regex.match(fullPath))

def is_male_starter(block: meta_file.FileBlock):
    fullPath = block.fullPath()
    return (not female_regex.match(fullPath)) and starter_regex.match(fullPath)

def is_male_non_starter(block: meta_file.FileBlock):
    fullPath = block.fullPath()
    return (not female_regex.match(fullPath)) and (not starter_regex.match(fullPath))

def generate_patch(outRootDir: pathlib.Path, sourcePath: str, targetLocationWithinOutDir: str | pathlib.Path, percent: float | None = None):
    outFile = outRootDir / targetLocationWithinOutDir
    outDir = outFile.parent
    outDir.mkdir(parents=True, exist_ok=True)
    shutil.copy(sourcePath, outFile)
    if percent is not None:
        bdo_utils.logi(f"\r\033[K  [{percent:6.2f}%] {targetLocationWithinOutDir} patched!", end="", flush=True)
    else:
        bdo_utils.logi(f"\r\033[K  {targetLocationWithinOutDir} patched!.", end="", flush=True)

def patch_with_dummy_model(outDir, targetLocationWithinOutDir : str | pathlib.Path, percent: float | None = None):
    dummy_pac = pathlib.Path(__file__).parent / "dummy.pac"
    generate_patch(outDir, dummy_pac, targetLocationWithinOutDir, percent)

def patch_with_blank_texture(outRootDir, targetLocationWithinOutDir, originalFileSize):
    # Choose a blank texture file based on size of the original texture
    if originalFileSize < 1000000:
        source = pathlib.Path(__file__).parent / "blank_dxt1.dds"
    else:
        source = pathlib.Path(__file__).parent / "blank_dxt5.dds"
    generate_patch(outRootDir, source, targetLocationWithinOutDir)

def patch_with_dummy_ao_texture(outRootDir, targetLocationWithinOutDir, originalFileSize):
    source = pathlib.Path(__file__).parent / "blank_ao.dds"
    # if "pdw_00_uw_0001_ao.dds" in str(targetLocationWithinOutDir):
    #     source = pathlib.Path(__file__).parent / "pdw_00_uw_0001_ao.dds"
    generate_patch(outRootDir, source, targetLocationWithinOutDir)

def patch_models(outDir: pathlib.Path, meta: meta_file.MetaFile, additional_requirement: callable = None):
    # search through model files of all classes, select every .pac files under player folder with name "38_underwear"
    bdo_utils.logi("Patching underwear models...")
    patched = []
    for block in meta.fileBlocks:
        # folder name needs to contain both "1_pc" and "38_underwear"
        if "1_pc" not in block.folderName:
            continue
        if "38_underwear" not in block.folderName:
            continue
        # do not touch Shi class
        if "14_plw" in block.folderName:
            continue
        # file name must end with ".pac"
        if not block.fileName.endswith(".pac"):
            continue
        # do additional checks
        if additional_requirement is not None and not additional_requirement(block):
            continue
        # found it. patch it with an the dummy pac file
        patch_with_dummy_model(outDir, block.fullPath())
        patched += [block]
    bdo_utils.logi(f"\n  {len(patched)} models patched.")

    # Generate .partcutdesc_exclusions.txt to match all patched models
    with open(outDir / ".partcutdesc_exclusions.txt", "w") as f:
        for block in patched:
            f.write(str(block.fullPath()).replace("\\", "/").replace(".pac", "").replace("character/model/", "") + "\n")

def patch_textures(outDir: pathlib.Path, meta: meta_file.MetaFile, additional_requirement: callable = None):
    pattern = re.compile(r"_\d{2}_uw_|_99_ub_")

    def is_underwear_texture(fileName):
        # do not touch Shi class
        if "plw_" in fileName:
            return False, False
        # file name must contain either "_##_uw_" or "_99_ub_"
        if not pattern.search(fileName):
            return False, False
        # Check if it is a ao texture. If yes, no future check is needed. We need to patch it.
        if "_ao.dds" in fileName:
            return True, True
        # We don't patch underwear/lb textures with these names: "_n.dds", "_sp.dds", "_m.dds", "_st.dds", "_fur"\
        # Note: this logic is copied from resorepless source code. Might need revisit.
        if "_n.dds" in fileName:
            return False, False
        if "_sp.dds" in fileName:
            return False, False
        if "_m.dds" in fileName:
            return False, False
        if "_st.dds" in fileName:
            return False, False
        if "_fur" in fileName:
            return False, False
        # Yes, this is a underwear texture (but not AO) that we need to patch.
        return True, False

    # search all underwear textures. replace it with a dummy texture
    bdo_utils.logi("Patching underwear textures...")
    counter = 0
    for block in meta.fileBlocks:
        # folder name must contain "character\texture"
        if "character/texture" not in block.folderName:
            continue
        # file name must end with ".dds"
        if not block.fileName.endswith(".dds"):
            continue

        # We only care about underwear textures
        needToPath, isAO = is_underwear_texture(block.fileName)
        if not needToPath:
            continue

        # do additional checks
        if additional_requirement is not None and not additional_requirement(block):
            continue

        # found it. replace it with an blank or dummy texture
        fullFilePath = block.fullPath()
        if isAO:
            patch_with_dummy_ao_texture(outDir, fullFilePath, block.size)
        else:
            patch_with_blank_texture(outDir, fullFilePath, block.size)

        # Done
        counter += 1
    bdo_utils.logi(f"\n  {counter} textures patched.")


def remove_underwear(outDir: pathlib.Path, meta: meta_file.MetaFile):
    patch_models(outDir, meta)
    patch_textures(outDir, meta)
    # # female starter
    # patch_models(outDir / "_female_starter", meta, is_female_starter)
    # patch_textures(outDir / "_female_starter", meta, is_female_starter)

    # # female non-starter
    # patch_models(outDir / "_female_non_starter", meta, is_female_non_starter)
    # patch_textures(outDir / "_female_non_starter", meta, is_female_non_starter)

    # # male starter
    # patch_models(outDir / "_male_starter", meta, is_male_starter)
    # patch_textures(outDir / "_male_starter", meta, is_male_starter)

    # # male non-starter
    # patch_models(outDir / "_male_non_starter", meta, is_male_non_starter)
    # patch_textures(outDir / "_male_non_starter", meta, is_male_non_starter)

    # generate a readme file
    with open(outDir / ".README.md", "w") as f:
        f.write(
            """# What's this?

This folder contains patch files to remove underwear model and textures from
all player classes up to release of Scholar.

# How to use it?

- Put it under files_to_patch folder.
- Run PartCutGen.exe to refresh your partcuts_desc.xml
- Run Meta Injector.exe to patch your game.

"""
        )
