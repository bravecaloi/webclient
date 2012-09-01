################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../controller/webclient.c 

OBJS += \
./controller/webclient.o 

C_DEPS += \
./controller/webclient.d 


# Each subdirectory must supply rules for building sources it contributes
controller/%.o: ../controller/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include -I/usr/lib/gcc/x86_64-linux-gnu/4.4.3/include -I/usr/lib/gcc/x86_64-linux-gnu/4.4/include-fixed -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


