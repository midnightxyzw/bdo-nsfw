#!/usr/bin/python3

import sys, pathlib, ctypes, pathlib
from bin import bdo_utils


class FileBlock:
    def __init__(self, meta_file):
        # Each file block is 28 bytes. The first 4 bytes is the hash value of the block.
        self.hash = int.from_bytes(meta_file.read(4), byteorder="little")
        # Next 4 bytes is folder number.
        self.folderNum = int.from_bytes(meta_file.read(4), byteorder="little")
        # Next 4 bytes is file number.
        self.fileNum = int.from_bytes(meta_file.read(4), byteorder="little")
        # Next 4 bytes is paz number.
        self.pazNum = int.from_bytes(meta_file.read(4), byteorder="little")
        # Next 4 bytes is file offset.
        self.fileOffset = int.from_bytes(meta_file.read(4), byteorder="little")
        # Next 4 bytes is zsize.
        self.zsize = int.from_bytes(meta_file.read(4), byteorder="little")
        # Next 4 bytes is size.
        self.size = int.from_bytes(meta_file.read(4), byteorder="little")

    def fullPath(self):
        return pathlib.Path(self.folderName) / self.fileName


class IceDecipher:
    def __init__(self):
        # Get directory of the script
        script_dir = pathlib.Path(__file__).parent.absolute()

        # Determine shared library extension
        if sys.platform == "win32":
            ext = ".dll"
        elif sys.platform == "linux":
            ext = ".so"
        else:
            ext = ".dylib"  # macOS

        # Load ice shared library
        self.lib = ctypes.CDLL(str(script_dir / "ice" / f"ice_decipher{ext}"))

        # define the function signature
        self.lib.decrypt_inplace.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.c_int]
        self.lib.decrypt_inplace.restype = None

    def decrypt(self, encrypted_data):
        c_byte_array = (ctypes.c_ubyte * len(encrypted_data)).from_buffer_copy(encrypted_data)
        self.lib.decrypt_inplace(c_byte_array, len(c_byte_array))
        return bytes(c_byte_array)


class MetaFile:
    def __init__(self, paz_folder: pathlib.Path):
        meta_file = paz_folder / "pad00000.meta"
        if not meta_file.exists():
            bdo_utils.rip(f"{meta_file} does not exist. Make sure you are pointing to BDO's PAZ folder.")
        bdo_utils.logi(f"Reading {meta_file}")
        with open(meta_file, "rb") as f:
            self.read_meta_file(f)

    def read_meta_file(self, f):
        ice = IceDecipher()

        expectedFileBlockCount = self.read_file_header(f)
        # Remember where we are in the file. When we read file block entries backwards, we can't go beyond this point.
        start = f.tell()

        # Search for an known block. This will give us a starting point to read file blocks.
        middle = self.search_for_the_known_block(f)

        # read forwards
        remaining, blockEnd = self.read_file_blocks_forward(f, middle, expectedFileBlockCount)

        # read backwards
        remaining = self.read_file_blocks_backward(f, start, middle, remaining, expectedFileBlockCount)

        # We have read all we can. Check if there's anything still missing.
        if remaining > 0:
            bdo_utils.logw(f"Could not read all file blocks. {remaining} file blocks are missing.")

        # Done reading file blocks. Print out how many we have read.
        bdo_utils.logi(f"  Number of file blocks read: {len(self.fileBlocks)}")

        # Decipher folder names, starting from where the file block table ends.
        offset = self.decipher_folder_names(f, ice, blockEnd)

        # Decipher file names, starting from where the folder names end.
        offset = self.decipher_file_names(f, ice, offset)

    def read_file_header(self, f):
        # skip the first 4 bytes of the file.
        f.seek(4)
        # read the next 4 bytes to get the number of paz entries
        self.pazCount = int.from_bytes(f.read(4), byteorder="little")
        # each paz entry is 12 bytes. let's skip them.
        f.seek(12 * self.pazCount, 1)
        # next 4 bytes is the number of file blocks.
        expectedFileBlockCount = int.from_bytes(f.read(4), byteorder="little")

        # print header information to console.
        bdo_utils.logi(f"  Number of paz entries: {self.pazCount}")
        bdo_utils.logi(f"  Number of file blocks expected: {expectedFileBlockCount}")

        # Done
        return expectedFileBlockCount

    def search_for_the_known_block(self, f):
        # The actual start offset of files block table is unknown. We need to search for it.
        # The way we do it is by searching for an known hash value of 631490897 that we know must exists.
        # It might be it the middle of the file block entries.
        while True:
            if int.from_bytes(f.read(4), byteorder="little") == 631490897:
                break

        # Seek back 4 bytes. Now we are at the beginning of the block.
        f.seek(-4, 1)

        # Return this point to the caller.
        return f.tell()

    def read_file_blocks_forward(self, f, start, expectedFileBlockCount):
        self.fileBlocks = []

        # start reading block by block, until the following ending conditions are met:
        # 1. We have read all the file blocks, or
        # 2. The block's fileNum is out of range of [0, expectedFileBlockCount)
        blockEnd = 0
        while len(self.fileBlocks) < expectedFileBlockCount:
            newBlock = FileBlock(f)

            # We'll stop as soon as we encounter an invalid block,.
            if newBlock.fileNum < 0 or newBlock.fileNum >= expectedFileBlockCount:
                break

            # this seems like a valid file block. Let's add it to the list.
            self.fileBlocks.append(newBlock)

            # update block end position
            blockEnd = f.tell()

        # Done.
        return expectedFileBlockCount - len(self.fileBlocks), blockEnd

    def read_file_blocks_backward(self, f, start, end, remaining, expectedFileBlockCount):
        if remaining == 0:
            # We have read all the file blocks. Nothing to do.
            return 0

        f.seek(end)

        blocks = []

        # The file block size is 28 bytes.
        blockSize = 28

        # Read in reverse order until we reach the start of the file entries:
        # 1. We have read all the file blocks (self.originalFileBlockCount), or
        # 2. The block's fileNum is out of range of [0, self.originalFileBlockCount)
        # 3. We have reached the self.originalFileBlockStart
        while remaining > 0:
            # Let's check if we have reached the start of the file entries.
            if (f.tell() - blockSize) < start:
                # We have passed over the starting point. Let's break the loop.
                break

            # Seek back one block.
            f.seek(-blockSize, 1)
            # Read it.
            newBlock = FileBlock(f)
            if newBlock.fileNum < 0 or newBlock.fileNum >= expectedFileBlockCount:
                # This is not a valid file block. We are done.
                break
            # This seems like a valid file block. Let's add it to the list
            blocks.append(newBlock)

            # seek back to the start of the block.
            f.seek(-blockSize, 1)

        # Attach the blocks to the beginning of the file blocks list.
        self.fileBlocks = blocks + self.fileBlocks

        # Done.
        return expectedFileBlockCount - len(self.fileBlocks)

    def decipher_folder_names(self, f, ice, startOffset):
        f.seek(startOffset)

        # Read the encrypted folder names and decrypt them.
        bdo_utils.logi("  Read and decrypt folder names...")
        length = int.from_bytes(f.read(4), byteorder="little")
        encrypted = f.read(length)
        bdo_utils.logi(f"  Encrypted folder names length: {length}")
        decrypted = ice.decrypt(encrypted)
        folderNames = self.splitFolderNames(decrypted)
        bdo_utils.logi(f"  Number of folder names decrypted: {len(folderNames)}")

        # Update folder names for all blocks
        numFolders = len(folderNames)
        for block in self.fileBlocks:
            block.folderName = folderNames[block.folderNum]

        # done.
        return f.tell()

    def splitFolderNames(self, decrypted):
        # We need to split the packed buffer into individual folder names.
        # Each folder name contains 8 bytes hash, followed by a null terminated string.
        # The hash is not used in the game. We will ignore it.
        folderNames = []
        offset = 8  # Skip the first hash.
        length = len(decrypted)
        while offset < length:
            # remember the current start point
            begin = offset
            # find the next null terminator
            while offset < length and decrypted[offset] != 0:
                offset += 1
            if offset < length:
                # extract the folder name
                s = decrypted[begin:offset]
            else:
                # this is the very last folder name, which is not null-terminated.
                s = decrypted[begin:]
            # append the folder name list
            folderNames.append(bdo_utils.decodeBinaryString(s))
            # move to the next folder name
            offset += 9

        return folderNames

    def decipher_file_names(self, f, ice, startOffset):
        f.seek(startOffset)

        # Read the encrypted file names and decrypt them.
        bdo_utils.logi("  Read and decrypt file names...")
        encryptedLength = int.from_bytes(f.read(4), byteorder="little")
        encrypted = f.read(encryptedLength)
        decrypted = ice.decrypt(encrypted)
        fileNames = decrypted.split(b"\x00")
        bdo_utils.logi(f"  Number of file names decrypted: {len(fileNames)}")

        # Update folder names for all blocks
        for block in self.fileBlocks:
            block.fileName = bdo_utils.decodeBinaryString(fileNames[block.fileNum])

        # done.
        return f.tell()
