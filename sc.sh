
#!/bin/bash

for i in `seq 1 3`;
do

./waf --run "scratch/lbt_test --txTime=$i" > logfile
grep 'Mean delay' logfile | awk '{print $3}' 

        done


