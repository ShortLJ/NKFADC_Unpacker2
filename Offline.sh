

set -x


RUNNAME=$1
RUNNUMBER=$(printf '%04d' $2)
PORT=$((8180+$3))
TIME=`date +"%Y%m%d_%H%M"`

INPUT=../../Data/KO_25_01/Run${RUNNUMBER}/FADC0.dat
OUTPUT_DIR=../../Unpacked_Jaehwan
HISTOUT=Hist_${RUNNAME}_${RUNNUMBER}_${TIME}.root
TREEOUT=Tree_${RUNNAME}_${RUNNUMBER}_${TIME}.root

./NKFADC_Unpacker -i ${INPUT} -tw 3000 -hp ${PORT} -ho ${OUTPUT_DIR}/${HISTOUT} -to ${OUTPUT_DIR}/${TREEOUT}

#./NKFADC_Unpacker -i online -tw 8 -ho 20251019_1840_152Eu_0.root
