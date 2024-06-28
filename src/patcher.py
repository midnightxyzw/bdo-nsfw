import pathlib, shutil, re, fnmatch
import meta_file, remove_underwear
from bin import bdo_utils


def generate_patch(outRootDir: pathlib.Path, sourcePath: str, targetLocationWithinOutDir: str | pathlib.Path, percent: float | None = None):
    outFile = outRootDir / targetLocationWithinOutDir
    outDir = outFile.parent
    outDir.mkdir(parents=True, exist_ok=True)
    shutil.copy(sourcePath, outFile)
    if percent is not None:
        bdo_utils.logi(f"\r\033[K  [{percent:6.2f}%] {targetLocationWithinOutDir} patched!", end="", flush=True)
    else:
        bdo_utils.logi(f"\r\033[K  {targetLocationWithinOutDir} patched!.", end="", flush=True)


def patch_with_dummy_model(outDir, targetLocationWithinOutDir: str | pathlib.Path, percent: float | None = None):
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


def hide_player_models(what: str, outDir: pathlib.Path, meta: meta_file.MetaFile, inclusions: callable = None, exclusions: callable = None):
    # search through model files of all classes
    bdo_utils.logi(what)
    position = 0
    total = len(meta.fileBlocks)
    patched = []
    for block in meta.fileBlocks:
        position += 1
        # folder name needs to contain "1_pc"
        if "1_pc" not in block.folderName:
            continue
        # do not touch Shi class
        if "14_plw" in block.folderName:
            continue
        # check against inclusion conditions, if not met, skip this block
        if inclusions is not None and not inclusions(block):
            continue
        # check against exclusion conditions, if met, skip this block.
        if exclusions is not None and exclusions(block):
            continue
        # found it. hide it by replacing it with a dummy mesh.
        patch_with_dummy_model(outDir, block.fullPath(), percent=position * 100.0 / total)
        patched += [block]
    bdo_utils.logi(f"\n  {len(patched)} models patched.")

    # Generate .partcutdesc_exclusions.txt to match all patched models
    if len(patched) > 0:
        with open(outDir / ".partcutdesc_exclusions.txt", "w") as f:
            for block in patched:
                f.write(str(block.fullPath()).replace("\\", "/").replace(".pac", "").replace("character/model/", "") + "\n")


def patch_player_ao_textures(what: str, outDir: pathlib.Path, meta: meta_file.MetaFile, inclusions: callable = None, exclusions: callable = None):
    # search through all character textures. replace it with a dummy texture
    bdo_utils.logi(what)
    counter = 0
    for block in meta.fileBlocks:
        # folder name must contain "character/texture"
        if "character/texture" not in block.folderName:
            continue
        # do not touch Shi class
        if "plw_" in block.fileName:
            continue
        # only care about AO textures
        if not block.fileName.endswith("_ao.dds"):
            continue
        # check against inclusion patterns.
        if inclusions is not None and not inclusions(block):
            continue
        if exclusions is not None and exclusions(block):
            continue
        # patch it with a dummy ao texture
        fullFilePath = block.fullPath()
        patch_with_dummy_ao_texture(outDir, fullFilePath, block.size)
        counter += 1
    bdo_utils.logi(f"\n  {counter} textures patched.")
