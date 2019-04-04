################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/laa-wifi-coexistence/examples/laa-wifi-80211ax-pathloss.cc \
../src/laa-wifi-coexistence/examples/laa-wifi-indoor.cc \
../src/laa-wifi-coexistence/examples/laa-wifi-itu-inh-pathloss.cc \
../src/laa-wifi-coexistence/examples/laa-wifi-itu-umi-pathloss.cc \
../src/laa-wifi-coexistence/examples/laa-wifi-outdoor.cc \
../src/laa-wifi-coexistence/examples/laa-wifi-simple.cc \
../src/laa-wifi-coexistence/examples/wifi-co-channel-networks.cc 

CC_DEPS += \
./src/laa-wifi-coexistence/examples/laa-wifi-80211ax-pathloss.d \
./src/laa-wifi-coexistence/examples/laa-wifi-indoor.d \
./src/laa-wifi-coexistence/examples/laa-wifi-itu-inh-pathloss.d \
./src/laa-wifi-coexistence/examples/laa-wifi-itu-umi-pathloss.d \
./src/laa-wifi-coexistence/examples/laa-wifi-outdoor.d \
./src/laa-wifi-coexistence/examples/laa-wifi-simple.d \
./src/laa-wifi-coexistence/examples/wifi-co-channel-networks.d 

OBJS += \
./src/laa-wifi-coexistence/examples/laa-wifi-80211ax-pathloss.o \
./src/laa-wifi-coexistence/examples/laa-wifi-indoor.o \
./src/laa-wifi-coexistence/examples/laa-wifi-itu-inh-pathloss.o \
./src/laa-wifi-coexistence/examples/laa-wifi-itu-umi-pathloss.o \
./src/laa-wifi-coexistence/examples/laa-wifi-outdoor.o \
./src/laa-wifi-coexistence/examples/laa-wifi-simple.o \
./src/laa-wifi-coexistence/examples/wifi-co-channel-networks.o 


# Each subdirectory must supply rules for building sources it contributes
src/laa-wifi-coexistence/examples/%.o: ../src/laa-wifi-coexistence/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


