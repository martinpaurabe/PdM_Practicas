################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f401retx.s 

OBJS += \
./Core/Startup/startup_stm32f401retx.o 

S_DEPS += \
./Core/Startup/startup_stm32f401retx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/martin/Desktop/MESE/PdM_Practicas/FinalProject/Bat_Charge_Monitor/Drivers/API" -I"/home/martin/Desktop/MESE/PdM_Practicas/FinalProject/Bat_Charge_Monitor/Drivers/API/Inc" -I"/home/martin/Desktop/MESE/PdM_Practicas/FinalProject/Bat_Charge_Monitor/Drivers/API/Src" -I"/home/martin/Desktop/MESE/PdM_Practicas/FinalProject/Bat_Charge_Monitor/Drivers/BCM" -I"/home/martin/Desktop/MESE/PdM_Practicas/FinalProject/Bat_Charge_Monitor/Drivers/BCM/Inc" -I"/home/martin/Desktop/MESE/PdM_Practicas/FinalProject/Bat_Charge_Monitor/Drivers/BCM/Src" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f401retx.d ./Core/Startup/startup_stm32f401retx.o

.PHONY: clean-Core-2f-Startup

