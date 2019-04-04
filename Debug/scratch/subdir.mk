################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../scratch/laa-wifi-indoor.cc \
../scratch/laa-wifi-simple.cc \
../scratch/lena-simple.cc \
../scratch/scratch-simulator.cc \
../scratch/wifi-co-channel-networks.cc 

CC_DEPS += \
./scratch/laa-wifi-indoor.d \
./scratch/laa-wifi-simple.d \
./scratch/lena-simple.d \
./scratch/scratch-simulator.d \
./scratch/wifi-co-channel-networks.d 

OBJS += \
./scratch/laa-wifi-indoor.o \
./scratch/laa-wifi-simple.o \
./scratch/lena-simple.o \
./scratch/scratch-simulator.o \
./scratch/wifi-co-channel-networks.o 


# Each subdirectory must supply rules for building sources it contributes
scratch/%.o: ../scratch/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


