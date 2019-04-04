# smart-lte-u
LTE-U simulation with machine learning on the NS-3 platform

Inital working Q-Learning model can be found in the following path:

smart-lte-u/src/laa-wifi-coexistence/model/basic-lbt-access-manager.cc

Compilation
cd ns-3-lbt
./waf configure -d optimized --enable-examples --enable-tests
./waf build
./test-lbt.py
