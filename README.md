# VSort

## About
VSort takes all files in a directory and sorts them based on file extension for easy organizing or better to organize afterwards.

## Usage
Just run `vsort -s` and (by default) it will create a "Sorted" directory and copy all the files in the current working directory into a Sorted/(File extension) Folder.

## Options
- Help menu (`--help`)
- Option to sort files based on file extension (`-s`)
- Option to use a advanced sorting algorithm instead of just sorting based file extension, so .mp4, .mkv, etc goes into a video folder and so on (`-a`)
- Option to exclude hidden files from being sorted (`-h`)
- Option to put all of the folders it makes in the current working directory instead of a "Sorted" folder (`-n`)
- All options can be used in combination to do even more advanced sorting, as a example, you can use `-a` and `-s` together to sort them based on type then by file extension afterwards.
