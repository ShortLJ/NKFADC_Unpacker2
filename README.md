# NKFADC_Unpacker2

## Quick start
```bash
mkdir build && cd build
cmake ..
cmake --build .
./NKFADC_Unpacker -i file.dat
```

## Unpack Options
```
--input,-i <file.dat> or 'online'\
--config,-c <config>\ ## The directory for *.csv files
--treeout,-to <tree.root>\
--histout,-ho <hists.root>\
--timewindow,-tw <timewindow=0> ## 8 ns for NKfadc\
--httpport, -hp <httpport=8181>\
```
Default values are presented in \<, \> brackets.

## Unpacking from a list
DEPRECIATED:
```
./NKFADC_Unpacker -i list.txt
```
is not suggested. It has problem when processing timestamp RESET (maximum hit, or MTE reboot). 

SUBSTITUTE:
```bash
./Offline_list.sh <batchname>.txt '<Options>' <N>
```
A user need to modify `INPUT_DATA_PATH`, and `OUTPUT_BASE` in the script.
`Options` argument is passed to `NKFADC_Unpacker2`.
Output `ROOT` files will be stored in directory `OUTPUT_BASE/batchname`.
Then, use `hadd`.

## Reading output tree
```bash
source <build_dir>/setup_nkunpacker.sh
```
for `LD_LIBRARY_PATH` and `ROOT_INCLUDE_PATH`. This script is configured by cmake.

## Sample macros
```
Test.C
Test.h
```
header files created by `MakeClass` function in `ROOT`.

```
Sample.c
pos_Energy.c
```
Simple macros presenting iteration to draw a histogram.
Feel free to ask anything.

```
tree_simple.c
draw_h2gg_rdataframe.c
```
Making a simple tree and drawing gamma-gamma histogram, which is being used for quick analysis by myself.


## User defined 'SIMPLE tree' (Experimental)
Change the symbolic links in <src> and <inc> directories, modify them in your favor, and complie again.
These are just examples to show a flexibility.
It is not suggested to have too complicated processing: there is no pause for the queing, no help in case of bottleneck in the tree writer. Still, if your computer can do it, you can do it.
