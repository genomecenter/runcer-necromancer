[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4340350.svg)](https://doi.org/10.5281/zenodo.4340350)

# Runcer-Necromancer

Data recovery tool for interrupted runs on MGISEQ-2000 (now DNBSEQ-G400)

## Purpose
Please refer to the our article for more information.

## Requirements
1. Linux (tested on Ubuntu)
2. GCC compiler with C++17 support
3. zlib (`apt-get install zlib1g-dev`)

## Building
Run build.sh script inside root folder

## Usage
### SaveReads
SaveReads program recovers sample files by placing fixed files into `fixed` directory inside current directory. It's important to check that there are no filenames clashes between samples files.

 SaveReads accepts `N+1` argument, where first argument is pool with reads from interrupted run, and next `N` arguments are standard samples files.

### SaveReads.py

To simplify call to SaveReads we wrote SaveReads.py script. This file accepts pool with reads as its single argument. All files with `_1.fq.gz` ending from current folder will be taken as samples files.
