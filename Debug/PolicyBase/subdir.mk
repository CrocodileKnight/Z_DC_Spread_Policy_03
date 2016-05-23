################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PolicyBase/MyCCI.cpp \
../PolicyBase/MyDMA.cpp \
../PolicyBase/MyEMA.cpp \
../PolicyBase/MyKDXZ.cpp \
../PolicyBase/MyMACD.cpp \
../PolicyBase/MyPUBU.cpp \
../PolicyBase/MyPUBU2.cpp \
../PolicyBase/MyRSI.cpp 

OBJS += \
./PolicyBase/MyCCI.o \
./PolicyBase/MyDMA.o \
./PolicyBase/MyEMA.o \
./PolicyBase/MyKDXZ.o \
./PolicyBase/MyMACD.o \
./PolicyBase/MyPUBU.o \
./PolicyBase/MyPUBU2.o \
./PolicyBase/MyRSI.o 

CPP_DEPS += \
./PolicyBase/MyCCI.d \
./PolicyBase/MyDMA.d \
./PolicyBase/MyEMA.d \
./PolicyBase/MyKDXZ.d \
./PolicyBase/MyMACD.d \
./PolicyBase/MyPUBU.d \
./PolicyBase/MyPUBU2.d \
./PolicyBase/MyRSI.d 


# Each subdirectory must supply rules for building sources it contributes
PolicyBase/%.o: ../PolicyBase/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/home/zhou/workspace/common -I/home/zhou/workspace/ctp_f_api -I/home/zhou/workspace/fema_api -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


