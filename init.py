# initialize the PAZ folder with the necessary files

import os, shutil, pathlib

def init_paz_folder():
    # make sure the current working folder is the root of the repository.
    os.chdir(pathlib.Path(__file__).parent)

    # copy "contrib/PartCutGen_EyelineCutFix_toolkit_1.3.0.exe" to PAZ folder and rename it to "PartCutGen.exe"
    shutil.copy("contrib/PartCutGen_EyelineCutFix_toolkit_1.3.0.exe", "PAZ/PartCutGen.exe")

    # copy "contrib/MetaInjector-1.4.1/Meta Injector.exe" to PAZ folder
    shutil.copy("contrib/MetaInjector-1.4.1/Meta Injector.exe", "PAZ/Meta Injector.exe")

    # copy everything in the "contrib/reporepless-v3.6f" folder to the PAZ folder
    shutil.copytree("contrib/resorepless-v3.6f/", "PAZ/", dirs_exist_ok=True)

if "__main__" == __name__:
    init_paz_folder()