# NKFADC_Unpacker2

## Unpacking from a list
DEPRECIATED:
```
./NKFADC_Unpacker -i list.txt
```
is not suggested. It has problem when processing timestamp RESET (maximum hit, or MTE reboot). 

SUBSTITUTE:
```bash
./Offline_list.sh <batchname>.txt <N>
```
A user need to modify `INPUT_DATA_PATH`, and `OUTPUT_BASE` in the script.
Output `ROOT` files will be stored in directory `OUTPUT_BASE/batchname`.
Then, use `hadd`.

## Reading output tree
```bash
source <build_dir>/setup_nkunpacker.sh
```
for `LD_LIBRARY_PATH` and `ROOT_INCLUDE_PATH`. This script is configured by cmake.
