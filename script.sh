#!/bin/bash

echo "test"
read users
./waf --run "scratch/lbt_test --txTime=$users" > logfile
echo grep 'Tx Packets\|Rx Packets' logfile | awk '{print $3}'

