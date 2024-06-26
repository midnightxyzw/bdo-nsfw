#!/usr/bin/python3

import pathlib, shutil, re
import meta_file

def generate_patch(outRootDir, sourcePath, targetLocationWithinOutDir):
    outFile = outRootDir / targetLocationWithinOutDir
    outDir = outFile.parent
    outDir.mkdir(parents=True, exist_ok=True)
    shutil.copy(sourcePath, outFile)

def patch_with_blank_texture(outRootDir, targetLocationWithinOutDir, originalFileSize):
    # Choose a blank texture file based on size of the original texture
    resorepless = pathlib.Path(__file__).parent.parent / "contrib/resorepless-v3.6f/patcher_resources/texture"
    if originalFileSize < 1000000:
        source =  resorepless / "blank_dxt1.dds"
    else:
        source = resorepless / "blank_dxt5.dds"
    generate_patch(outRootDir, source, targetLocationWithinOutDir)

def patch_with_dummy_ao_texture(outRootDir, targetLocationWithinOutDir, originalFileSize):
    resorepless = pathlib.Path(__file__).parent.parent / "contrib/resorepless-v3.6f/patcher_resources/texture"
    if "pdw_00_uw_0001_ao.dds" in str(targetLocationWithinOutDir):
        source = resorepless / "pdw_00_uw_0001_ao.dds"
    elif originalFileSize > 600000:
        source = resorepless / "blank_ao_683.dds"
    elif originalFileSize > 170000:
        source = resorepless / "blank_ao_171.dds"
    elif originalFileSize > 43000:
        source = resorepless / "blank_ao_43.dds"
    else:
        # default to 171 version.
        source = resorepless / "blank_ao_171.dds"
    generate_patch(outRootDir, source, targetLocationWithinOutDir)

def decodeBinaryString(binaryString):
    try:
        return binaryString.decode("ascii")
    except UnicodeDecodeError:
        try:
            s = binaryString.decode("euc-kr")
            meta_file.warning(f"Encountered a korean string: {s}.")
            return s
        except UnicodeDecodeError:
            meta_file.rip(f"Could not decode {binaryString}.")

def patch_models(outDir: pathlib.Path, meta: meta_file.MetaFile):
    # check if dummy pac file exists
    dummy_pac = pathlib.Path(__file__).parent.parent / "contrib/resorepless-v3.6f/patcher_resources/models/t0072_pumpkin_ground_0001.pac"
    if not dummy_pac.exists():
        meta_file.rip("dummy.pac does not exist. Make sure it is in the same folder as this script.")

    # search through model files of all classes, select every .pac files under player folder with name "38_underwear"
    meta_file.info("Patching underwear models...")
    models = []
    for block in meta.fileBlocks:
        # folder name needs to contain both "1_pc" and "38_underwear"
        if b"1_pc" not in block.folderName: continue
        if b"38_underwear" not in block.folderName: continue
        # do not touch Shi class
        if b"14_plw" in block.folderName: continue
        # file name must end with ".pac"
        if not block.fileName.endswith(b".pac"): continue
        # found it. place it with an the dummy pac file
        fullFilePath = pathlib.Path(decodeBinaryString(block.folderName)) / decodeBinaryString(block.fileName)
        generate_patch(outDir, dummy_pac, fullFilePath)
        meta_file.info(f"  {fullFilePath} patched!")
        models.append(block)

    # Generate .partcutdesc_exclusions.txt to match all patched models
    with open(outDir / ".partcutdesc_exclusions.txt", "w") as f:
        f.write("1_pc/*/38_underwear/\n")

def patch_textures(outDir: pathlib.Path, meta: meta_file.MetaFile):
    pattern = re.compile(r"_\d{2}_uw_|_99_ub_")
    def is_underwear_texture(fileName):
        # do not touch SHI class
        if "plw_" in fileName: return False, False
        # file name must contain either "_##_uw_" or "_99_ub_"
        if not pattern.search(fileName): return False, False
        # Check if it is a ao texture. If yes, no future check is needed. We need to patch it.
        if "_ao.dds" in fileName: return True, True
        # We don't patch underwear/lb textures with these names: "_n.dds", "_sp.dds", "_m.dds", "_st.dds", "_fur"
        if "_n.dds" in fileName: return False, False
        if "_sp.dds" in fileName: return False, False
        if "_m.dds" in fileName: return False, False
        if "_st.dds" in fileName: return False, False
        if "_fur" in fileName: return False, False
        # Yes, this is a underwear texture (but not AO) that we need to patch.
        return True, False

    # search all underwear textures. replace it with a dummy texture
    meta_file.info("Patching underwear textures...")
    textures = []
    for block in meta.fileBlocks:
        # folder name must contain "character\texture"
        if b"character/texture" not in block.folderName: continue
        # file name must end with ".dds"
        if not block.fileName.endswith(b".dds"): continue

        # Convert to pathlib.Path. Note that we have to do this after the checks above.
        # since some folder name contains non-ascii characters that we don't know how to decode.
        folder = pathlib.Path(decodeBinaryString(block.folderName))
        fileName = decodeBinaryString(block.fileName)

        # We only care about underwear textures
        needToPath, isAO = is_underwear_texture(fileName)
        if not needToPath: continue

        # found it. replace it with an blank or dummy texture
        fullFilePath = folder / fileName
        if isAO:
            patch_with_dummy_ao_texture(outDir, fullFilePath, block.size)
        else:
            patch_with_blank_texture(outDir, fullFilePath, block.size)

        # Done
        meta_file.info(f"  {fullFilePath} patched!.")
        textures.append(block)

def remove_underwear(outDir: pathlib.Path, meta: meta_file.MetaFile):
    patch_models(outDir, meta)
    patch_textures(outDir, meta)
    # generate a readme file
    with open(outDir / ".README.md", "w") as f:
        f.write("""# What's this?

This folder contains patch files to remove underwear model and textures from
all player classes up to release of Scholar.

# How to use it?

- Put it under files_to_patch folder.
- Run PartCutGen.exe to refresh your partcuts_desc.xml
- Run Meta Injector.exe to patch your game.

# Release Notes

- No nude mesh/skin contained here. It is intended to be used with your own
  collection of nude mods and armor patches.

- Generated by Midnight Xyzw's BDO NSFW patch.
""")
