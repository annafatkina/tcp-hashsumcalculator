# Run: `. run.sh <num of clients>`
# Runs tcp server and some netcat clients. Sends strings specified in 
# 'teststrings.txt' file from each 'nc' client, and saves the result to output
# files 'out<n>.tmp'.
# Warning: this script kills all nc and tcp_server processes in the end!

#!/bin/sh

./build/src/tcp_server 1234 &

sleep 1

for i in $(seq "$1")
do
    echo $i
    outfile="out$i.tmp"
    cat teststrings.txt | nc localhost 1234 &> $outfile &
done

sleep 1

killall nc
killall tcp_server
