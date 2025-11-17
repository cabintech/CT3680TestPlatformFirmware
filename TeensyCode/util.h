/*
 * util.h
 *
 *  Created on: Nov 12, 2025
 *      Author: Editor
 */

#ifndef UTIL_H_
#define UTIL_H_

/**
 * Data structure to hold results of getCV() function.
 */
struct CVData {
	int values[6];			// Raw FXCore CV value in range 0 to maximum of 0x7FFFFFFF
	float percent[6];		// Fractional percentage of CV maximum (e.g. 0.5 = 50%) range 0.0 to 1.0
	float volts[6];		// Calculated voltage in range 0 to 3.3
};

/**
 * Functions used by other source files
 */
void powerOn();
void powerOff();
void defaultModuleConfiguration();
float calculateVoltage(int reading);
void setPinModes();
void sendResponse(const char * response, ...);
void getModuleSN(char *buff);
void readModuleDiag(byte *buff);
bool checkForErrors();
int reverseBytes(const byte *buff);
float convertIntervalToMS(int interval, int rate);
void getCV(const char * cvName, struct CVData *cvData);

#endif /* UTIL_H_ */
