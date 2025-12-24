# NKFADC_Unpacker2

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

## User defined 'SIMPLE tree'
Change the symbolic links in <src> and <inc> directories, modify them in your favor, and complie again.
These are just examples to show a flexibility.
It is not suggested to have too complicated processing: there is no pause for the queing, no help in case of bottleneck in the tree writer. Still, if your computer can do it, you can do it.
