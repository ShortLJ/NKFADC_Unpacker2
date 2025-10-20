

set -x


RUNNAME=$1
RUNNUMBER=$(printf '%02d' $2)
PORT=$((8180+$3))
TIME=`date +"%Y%m%d_%H%M"`


./NKFADC_Unpacker -i online -tw 8 -hp ${PORT} -ho ../../rootData/Hist_${RUNNAME}_${RUNNUMBER}_${TIME}.root -to ../../rootData/Tree_${RUNNAME}_${RUNNUMBER}_${TIME}.root

#./NKFADC_Unpacker -i online -tw 8 -ho 20251019_1840_152Eu_0.root
