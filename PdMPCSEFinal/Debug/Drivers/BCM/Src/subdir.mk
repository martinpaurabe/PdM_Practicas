################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BCM/Src/Unit_BatChargeMonitor.c 

OBJS += \
./Drivers/BCM/Src/Unit_BatChargeMonitor.o 

C_DEPS += \
./Drivers/BCM/Src/Unit_BatChargeMonitor.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BCM/Src/%.o Drivers/BCM/Src/%.su Drivers/BCM/Src/%.cyclo: ../Drivers/BCM/Src/%.c Drivers/BCM/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/martin/Desktop/MESE/PdM_Practicas/PdMPCSEFinal/Drivers/API" -I"/home/martin/Desktop/MESE/PdM_Practicas/PdMPCSEFinal/Drivers/API/Inc" -I"/home/martin/Desktop/MESE/PdM_Practicas/PdMPCSEFinal/Drivers/API/Src" -I"/home/martin/Desktop/MESE/PdM_Practicas/PdMPCSEFinal/Drivers/BCM/Inc" -I"/home/martin/Desktop/MESE/PdM_Practicas/PdMPCSEFinal/Drivers/BCM/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BCM-2f-Src

clean-Drivers-2f-BCM-2f-Src:
	-$(RM) ./Drivers/BCM/Src/Unit_BatChargeMonitor.cyclo ./Drivers/BCM/Src/Unit_BatChargeMonitor.d ./Drivers/BCM/Src/Unit_BatChargeMonitor.o ./Drivers/BCM/Src/Unit_BatChargeMonitor.su

.PHONY: clean-Drivers-2f-BCM-2f-Src

