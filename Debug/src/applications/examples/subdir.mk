################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/applications/examples/voice-example.cc 

CC_DEPS += \
./src/applications/examples/voice-example.d 

OBJS += \
./src/applications/examples/voice-example.o 


# Each subdirectory must supply rules for building sources it contributes
src/applications/examples/%.o: ../src/applications/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


