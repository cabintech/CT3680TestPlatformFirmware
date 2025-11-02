#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2025-10-30 22:25:45

#include "Arduino.h"
#include "Arduino.h"
#include <cstring>
#include <LiquidCrystal_I2C.h>

void setPinModes() ;
void relayPower(bool turnOn) ;
void lcdPrintln(int lineNum, char const * str) ;
void clearModuleConfiguration() ;
void defaultModuleConfiguration() ;
void setup() ;
void waitForButtonRelease(int pinNum) ;
char waitForAnyButton(const char * prompt1, const char * prompt2) ;
void getModuleSN(char * buff) ;
void readModuleDiag() ;
void loop() ;

#include "CT3680TestPlatform.ino"


#endif
