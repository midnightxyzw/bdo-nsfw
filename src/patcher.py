import pathlib, shutil, abc
import meta_file
from bin import bdo_utils


class FileCategorizer(abc.ABC):
    @abc.abstractmethod
    def categorize(self, block: meta_file.FileBlock) -> str | None:
        """
        Returns the category name the block belongs to.
        Returns None, if the file should be ignored.
        Returns empty string if the file does not belong to any category.
        """
        pass


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


def categorize(outRootDir: pathlib.Path, fileBlock: meta_file.FileBlock, categorizer: FileCategorizer) -> pathlib.Path | None:
    category = categorizer.categorize(fileBlock)
    if category is None:  # ignore this file
        return None
    if category == "":
        return outRootDir
    else:
        if not category.startswith("_"):
            category = "_" + category
        return outRootDir / category


def hide_player_models(what: str, outRootDir: pathlib.Path, meta: meta_file.MetaFile, categorizer: FileCategorizer):
    # search through model files of all classes
    bdo_utils.logi(what)
    position = 0
    count = 0
    total = len(meta.fileBlocks)
    patched = {}
    for block in meta.fileBlocks:
        position += 1
        # folder name needs to contain "1_pc"
        if "1_pc" not in block.folderName:
            continue
        # do not touch Shi class
        if "14_plw" in block.folderName:
            continue
        # check against categorizer
        outDir = categorize(outRootDir, block, categorizer)
        if outDir is None:
            continue
        # Hide it by replacing it with a dummy mesh.
        patch_with_dummy_model(outDir, block.fullPath(), percent=position * 100.0 / total)
        if outDir in patched:
            patched[outDir].append(block)
        else:
            patched[outDir] = [block]
        count += 1
    bdo_utils.logi(f"\n  {count} models patched.")

    # Generate .partcutdesc_exclusions.txt to match all patched models
    for folder, items in patched.items():
        with open(folder / ".partcutdesc_exclusions.txt", "w") as f:
            for block in items:
                f.write(str(block.fullPath()).replace("\\", "/").replace(".pac", "").replace("character/model/", "") + "\n")


def patch_player_ao_textures(what: str, outRootDir: pathlib.Path, meta: meta_file.MetaFile, categorizer: FileCategorizer):
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
        # check against categorizer
        outDir = categorize(outRootDir, block, categorizer)
        if outDir is None:
            continue
        # patch it with a dummy ao texture
        patch_with_dummy_ao_texture(outDir, block.fullPath(), block.size)
        counter += 1
    bdo_utils.logi(f"\n  {counter} textures patched.")
