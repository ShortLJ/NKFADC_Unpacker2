
num=$1
port=$2
screen -dm -L -Logfile ../../Unpacked_Jaehwan/log/$num.txt -S unpack$num bash ../Offline.sh CD2 $num $port
