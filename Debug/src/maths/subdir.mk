################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/maths/PerlinNoise.cpp

OBJS += \
./src/maths/PerlinNoise.o

CPP_DEPS += \
./src/maths/PerlinNoise.d


# Each subdirectory must supply rules for building sources it contributes
src/window/%.o: ../src/window/%.cpp src/window/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
