################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\utility\twi.c 

C_DEPS += \
.\libraries\Wire\utility\twi.c.d 

LINK_OBJ += \
.\libraries\Wire\utility\twi.c.o 


# Each subdirectory must supply rules for building sources it contributes
libraries\Wire\utility\twi.c.o: C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\utility\twi.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:\Sloeber\arduinoPlugin\packages\teensy\tools\teensy-compile\11.3.1/arm/bin/arm-none-eabi-gcc" -c -O2 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD  -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -D__IMXRT1062__ -DTEENSYDUINO=159 -DARDUINO=10812 -DARDUINO_TEENSY41 -DF_CPU=24000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\cores\teensy4" -I"C:\Users\Editor\Documents\Arduino\libraries\LiquidCrystal_I2C" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\utility" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\cores\teensy4" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


