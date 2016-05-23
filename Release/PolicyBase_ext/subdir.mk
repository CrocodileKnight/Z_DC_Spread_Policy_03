################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PolicyBase_ext/MyDCM.cpp 

OBJS += \
./PolicyBase_ext/MyDCM.o 

CPP_DEPS += \
./PolicyBase_ext/MyDCM.d 


# Each subdirectory must supply rules for building sources it contributes
PolicyBase_ext/%.o: ../PolicyBase_ext/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/home/zhou/Include/common -I/home/zhou/Include/ctp_f_api -I/home/zhou/Include/fema_api -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


