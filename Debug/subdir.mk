################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../EXPolicy.cpp \
../Z_DC_KDXZ_Policy.cpp \
../pBase.cpp 

OBJS += \
./EXPolicy.o \
./Z_DC_KDXZ_Policy.o \
./pBase.o 

CPP_DEPS += \
./EXPolicy.d \
./Z_DC_KDXZ_Policy.d \
./pBase.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/home/zhou/workspace/common -I/home/zhou/workspace/ctp_f_api -I/home/zhou/workspace/fema_api -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


