################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/laa-wifi-coexistence/model/basic-lbt-access-manager.cc \
../src/laa-wifi-coexistence/model/duty-cycle-access-manager.cc \
../src/laa-wifi-coexistence/model/lbt-access-manager.cc 

CC_DEPS += \
./src/laa-wifi-coexistence/model/basic-lbt-access-manager.d \
./src/laa-wifi-coexistence/model/duty-cycle-access-manager.d \
./src/laa-wifi-coexistence/model/lbt-access-manager.d 

OBJS += \
./src/laa-wifi-coexistence/model/basic-lbt-access-manager.o \
./src/laa-wifi-coexistence/model/duty-cycle-access-manager.o \
./src/laa-wifi-coexistence/model/lbt-access-manager.o 


# Each subdirectory must supply rules for building sources it contributes
src/laa-wifi-coexistence/model/%.o: ../src/laa-wifi-coexistence/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


