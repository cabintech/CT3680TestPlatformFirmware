################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\Wire.cpp \
C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\WireIMXRT.cpp \
C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\WireKinetis.cpp 

LINK_OBJ += \
.\libraries\Wire\Wire.cpp.o \
.\libraries\Wire\WireIMXRT.cpp.o \
.\libraries\Wire\WireKinetis.cpp.o 

CPP_DEPS += \
.\libraries\Wire\Wire.cpp.d \
.\libraries\Wire\WireIMXRT.cpp.d \
.\libraries\Wire\WireKinetis.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\Wire\Wire.cpp.o: C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\Wire.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Sloeber\arduinoPlugin\packages\teensy\tools\teensy-compile\11.3.1/arm/bin/arm-none-eabi-g++" -c -O2 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -std=gnu++17 -fno-exceptions -fpermissive -fno-rtti -fno-threadsafe-statics -felide-constructors -Wno-error=narrowing -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -D__IMXRT1062__ -DTEENSYDUINO=159 -DARDUINO=10812 -DARDUINO_TEENSY41 -DF_CPU=24000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH "-IC:\Users\Editor\OneDrive\Documents\School\GCU\CST452\TeensyCode\Release/pch" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\cores\teensy4" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire" -I"C:\Users\Editor\Documents\Arduino\libraries\LiquidCrystal_I2C" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\Wire\WireIMXRT.cpp.o: C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\WireIMXRT.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Sloeber\arduinoPlugin\packages\teensy\tools\teensy-compile\11.3.1/arm/bin/arm-none-eabi-g++" -c -O2 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -std=gnu++17 -fno-exceptions -fpermissive -fno-rtti -fno-threadsafe-statics -felide-constructors -Wno-error=narrowing -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -D__IMXRT1062__ -DTEENSYDUINO=159 -DARDUINO=10812 -DARDUINO_TEENSY41 -DF_CPU=24000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH "-IC:\Users\Editor\OneDrive\Documents\School\GCU\CST452\TeensyCode\Release/pch" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\cores\teensy4" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire" -I"C:\Users\Editor\Documents\Arduino\libraries\LiquidCrystal_I2C" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\Wire\WireKinetis.cpp.o: C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\WireKinetis.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Sloeber\arduinoPlugin\packages\teensy\tools\teensy-compile\11.3.1/arm/bin/arm-none-eabi-g++" -c -O2 -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -std=gnu++17 -fno-exceptions -fpermissive -fno-rtti -fno-threadsafe-statics -felide-constructors -Wno-error=narrowing -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -D__IMXRT1062__ -DTEENSYDUINO=159 -DARDUINO=10812 -DARDUINO_TEENSY41 -DF_CPU=24000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH "-IC:\Users\Editor\OneDrive\Documents\School\GCU\CST452\TeensyCode\Release/pch" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\cores\teensy4" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire" -I"C:\Users\Editor\Documents\Arduino\libraries\LiquidCrystal_I2C" -I"C:\Sloeber\arduinoPlugin\packages\teensy\hardware\avr\1.59.0\libraries\Wire\utility" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


