################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/laa-wifi-coexistence/helper/laa-wifi-coexistence-helper.cc \
../src/laa-wifi-coexistence/helper/scenario-helper.cc 

CC_DEPS += \
./src/laa-wifi-coexistence/helper/laa-wifi-coexistence-helper.d \
./src/laa-wifi-coexistence/helper/scenario-helper.d 

OBJS += \
./src/laa-wifi-coexistence/helper/laa-wifi-coexistence-helper.o \
./src/laa-wifi-coexistence/helper/scenario-helper.o 


# Each subdirectory must supply rules for building sources it contributes
src/laa-wifi-coexistence/helper/%.o: ../src/laa-wifi-coexistence/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


