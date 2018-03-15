################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Events.c \
../Sources/application.c \
../Sources/communication.c \
../Sources/custom_queue.c \
../Sources/drive.c \
../Sources/driveTelescope.c \
../Sources/main.c 

OBJS += \
./Sources/Events.o \
./Sources/application.o \
./Sources/communication.o \
./Sources/custom_queue.o \
./Sources/drive.o \
./Sources/driveTelescope.o \
./Sources/main.o 

C_DEPS += \
./Sources/Events.d \
./Sources/application.d \
./Sources/communication.d \
./Sources/custom_queue.d \
./Sources/drive.d \
./Sources/driveTelescope.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/Users/El_Matador/Desktop/FreedomBoard/Kinetis_Design_Studio/Seilbahn/Static_Code/PDD" -I"C:/Users/El_Matador/Desktop/FreedomBoard/Kinetis_Design_Studio/Seilbahn/Static_Code/IO_Map" -I"C:/Users/El_Matador/Desktop/FreedomBoard/Kinetis_Design_Studio/Seilbahn/Sources" -I"C:/Users/El_Matador/Desktop/FreedomBoard/Kinetis_Design_Studio/Seilbahn/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


