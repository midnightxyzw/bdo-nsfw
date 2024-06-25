#!/usr/bin/python3

import sys, argparse, pathlib
from termcolor import colored

def info(message):
    # print to stdout
    print(f"{message}")

def warning(message):
    # print to stderr
    sys.stderr.write(colored(f"[WARNING] {message}\n", "yellow"))

def rip(message):
    # print to stderr and exit
    sys.stderr.write(colored(f"[FATAL] {message}\n", "red"))
    sys.exit(1)

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
        

class MetaFile:
    def __init__(self, paz_folder):
        meta_file =  pathlib.Path(paz_folder) / "pad00000.meta"
        if not meta_file.exists():
            rip(f"{meta_file} does not exist. Make sure you are pointing to BDO's PAZ folder.")
        with open(meta_file, "rb") as f:
            expectedFileBlockCount = self.read_file_header(f)
            # Remember where we are in the file. When we read file block entries backwards, we can't go beyond this point.
            start = f.tell()

            # Search for an known block. This will give us a starting point to read file blocks.
            middle = self.searchForAnKnownBlock(f)

            # read forwards
            remaining = self.readFileBlocksForward(f, middle, expectedFileBlockCount)

            # read backwards
            remaining = self.readFileBlocksBackward(f, start, middle, remaining, expectedFileBlockCount)

            # We have read all we can. Check if there's anything still missing.
            if remaining > 0:
                warning(f"Could not read all file blocks. {remaining} file blocks are missing.")

        # Done reading file blocks. Print out how many we have read.
        info(f"Number of file blocks read: {len(self.fileBlocks)}")

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
        info(f"Number of paz entries: {self.pazCount}")
        info(f"Number of file blocks expected: {expectedFileBlockCount}")

        # Done
        return expectedFileBlockCount

    def searchForAnKnownBlock(self, f):
        # The actual start offset of files block table is unknown. We need to search for it.
        # The way we do it is by searching for an known hash value of 631490897 that we know must exists.
        # It might be it the middle of the file block entries.
        while True:
            if int.from_bytes(f.read(4), byteorder="little") == 631490897:
                break

        # Seek back 4 bytes. Now we are at the bebinning of the block.
        f.seek(-4, 1)

        # Return this point to the caller.
        return f.tell()
    
    def readFileBlocksForward(self, f, start, expectedFileBlockCount):
        self.fileBlocks = []

        # startin reading block by block, until the following ending conditions are met:
        # 1. We have read all the file blocks, or
        # 2. The block's fileNum is out of range of [0, expectedFileBlockCount)
        while len(self.fileBlocks) < expectedFileBlockCount:
            newBlock = FileBlock(f)

            # We'll stop as soon as we encounter an invalid block,.
            if newBlock.fileNum < 0 or newBlock.fileNum >= expectedFileBlockCount:
                break

            # this seems like a valie file block. Let's add it to the list.
            self.fileBlocks.append(newBlock)

        # Done.
        return expectedFileBlockCount - len(self.fileBlocks)
    
    def readFileBlocksBackward(self, f, start, end, remaining, expectedFileBlockCount):
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

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("-i", required=True, help="Path to your BDO's PAZ folder")
    ap.add_argument("-o", help="Path to the output folder. Default is the current working directory.")
    args = ap.parse_args()
    meta = MetaFile(args.i)
    # patch = remove_underwear(meta)
    # write_patch(patch, args.o)

