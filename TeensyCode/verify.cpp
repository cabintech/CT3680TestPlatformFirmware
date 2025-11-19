#include "Arduino.h"
#include <stdio.h>
#include "CT3680TestPlatform.h"
#include "util.h"
#include "verify.h"

//Verifies the the diagnostic stream is running
void verifyDiagStream() {
	byte buff[DIAG_MAX_BYTES];
	readModuleDiag(buff);
}

void verifyOptionSwitches() {
	//resetState();
	int testPin;
	int testValue;
	for (int i = 0; i < 5; i++) {
		switch (i) {
		case 0: {
			testPin = PIN_OPTION1;
			testValue = 0b11110;
			break;
		}
		case 1: {
			testPin = PIN_OPTION2;
			testValue = 0b11101;
			break;
		}
		case 2: {
			testPin = PIN_OPTION3;
			testValue = 0b11011;
			break;
		}
		case 3: {
			testPin = PIN_OPTION4;
			testValue = 0b10111;
			break;
		}
		case 4: {
			testPin = PIN_OPTION5;
			testValue = 0b01111;
			break;
		}
		default: {
			break;
		}
		}

		digitalWrite(testPin, LOW);
		delay(100);
		if (checkForErrors()) {
			return;
		}

		//create buffer
		byte buff[DIAG_MAX_BYTES];
		readModuleDiag(buff);

		//verify option values
		if (buff[52] != testValue) {
			sprintf(errorMsg, "SET OPTION SWITCH failed, pin number %i. Should be %i, was %i",
					testPin, testValue, (int)buff[52]);
			return;
		}

		digitalWrite(testPin, HIGH);
		delay(100);
		readModuleDiag(buff);
		if (checkForErrors()) {
			return;
		}

		//verify all options are HIGH
		if (buff[52] != 0b11111) {
			sprintf(errorMsg,
					"OPTION SWITCH failed to reset to HIGH, pin number %i",
					testPin);
			return;
		}
	}

	return;
}

void verifyPGMSwitches() {
	//resetState();
	for (int i = 0; i < 16; i++) {
		//Serial.print("Setting program ");
		//Serial.println(i);
		if ((i & 0b1000) == 0b1000) {
			digitalWrite(PIN_PGM3, HIGH);
			//Serial.println("Setting PGM3 HIGH");
		} else {
			digitalWrite(PIN_PGM3, LOW);
			//Serial.println("Setting PGM3 LOW");
		}
		if ((i & 0b0100) == 0b0100) {
			digitalWrite(PIN_PGM2, HIGH);
			//Serial.println("Setting PGM2 HIGH");
		} else {
			digitalWrite(PIN_PGM2, LOW);
			//Serial.println("Setting PGM2 LOW");
		}
		if ((i & 0b0010) == 0b0010) {
			digitalWrite(PIN_PGM1, HIGH);
			//Serial.println("Setting PGM1 HIGH");
		} else {
			digitalWrite(PIN_PGM1, LOW);
			//Serial.println("Setting PGM1 LOW");
		}
		if ((i & 0b0001) == 0b0001) {
			digitalWrite(PIN_PGM0, HIGH);
			//Serial.println("Setting PGM0 HIGH");
		} else {
			digitalWrite(PIN_PGM0, LOW);
			//Serial.println("Setting PGM0 LOW");
		}

		delay(100);
		//create buffer
		byte buff[DIAG_MAX_BYTES];
		readModuleDiag(buff);

		if (checkForErrors()) {
			return;
		}

		//get program data from diag stream
		int programSelection = buff[2];

		if (i < 8) {
			if (programSelection != i) {
				sprintf(errorMsg, "SET PROGRAM failed, expected program %i, read as program %i", i, programSelection);
				return;
			}
		} else {
			if (programSelection != 255) {
				sprintf(errorMsg, "SET PROGRAM failed, program %i", i);
				return;
			}
		}
	}
	return;
}

void verifySamplingRates() {
//	digitalWrite(PIN_RATE0, LOW);
//	digitalWrite(PIN_RATE1, LOW);
//	delay(100);
//	digitalWrite(PIN_RESET, LOW);
//	delay(500);
//	digitalWrite(PIN_RESET, HIGH);
//	delay(1000);
//	byte b[DIAG_MAX_BYTES];
//	readModuleDiag(b);
//	sprintf(errorMsg, "Byte 11 is %02X", (unsigned int)b[8]);
//	return;

	//resetState();
	for (int i = 0; i < 4; i++) {
		//Serial.print("Setting sample rate ");
		//Serial.println(i);
		if (i==2 || i==3) {
			digitalWrite(PIN_RATE1, HIGH);
		} else {
			digitalWrite(PIN_RATE1, LOW);
		}
		if (i==1 || i==3) {
			digitalWrite(PIN_RATE0, HIGH);
		} else {
			digitalWrite(PIN_RATE0, LOW);
		}
		delay(10);

		// Pulse reset LOW to do hardware reboot of the FXCore
		digitalWrite(PIN_RESET, LOW);
		delay(100);
		digitalWrite(PIN_RESET, HIGH);
		delay(1000); // Takes a while for reset to complete and the data stream to restart

		//get rate byte from diag stream
		byte buff[DIAG_MAX_BYTES];
		readModuleDiag(buff);

		if (checkForErrors()) {
			return;
		}

		int rateSelection = buff[8] & 0b00000011;

		if (rateSelection != i) {
			sprintf(errorMsg, "SET RATE failed, expected %i found %i", i, rateSelection);
			return;
		}
	}
	return;
}

void verifyTapTempo() {
	//resetState();

	//reset Tap Tempo
	digitalWrite(PIN_TT_IN, LOW);
	delay(1000);
	digitalWrite(PIN_TT_IN, HIGH);

	//Make two LOW pulses on TT pin
	digitalWrite(PIN_TT_IN, LOW);
	delay(50);
	digitalWrite(PIN_TT_IN, HIGH);
	delay(450);
	digitalWrite(PIN_TT_IN, LOW);
	delay(50);

	//get rate bytes from diag stream
	byte buff[DIAG_MAX_BYTES];
	readModuleDiag(buff);

	delay(1);
	if (checkForErrors()) {
		return;
	}

	//verify TT in diag stream
	int ttData = reverseBytes(buff + 60);
	if (ttData == 0) {
		sprintf(errorMsg, "TAP TEMPO DISENGAGED");
		return;
	}

	//get sampling rate
	int rateSelection = buff[11] & 0b00000011;

	float delayInMS = convertIntervalToMS(ttData, rateSelection);
	if (delayInMS > (delayInMS * 1.1) || delayInMS < (delayInMS * 0.9)) {
		sprintf(errorMsg, "Measured interval not consistent with known delay");
		return;
	}

	//reset Tap Tempo again
	digitalWrite(PIN_TT_IN, LOW);
	delay(1000);
	digitalWrite(PIN_TT_IN, HIGH);

	return;
}

//not yet coded
void verifyVCSettings() {

	analogWrite(PIN_CV_D1, 0);
	analogWrite(PIN_CV_D2, 0);
	analogWrite(PIN_CV_D3, 0);
	analogWrite(PIN_CV_D4, 0);
	analogWrite(PIN_CV_MIN, 0);
	analogWrite(PIN_CV_MAX, 0);


	int testPin;
	for (int i = 0; i < 6; i++) {
		switch (i) {
		case 0: {
			testPin = PIN_CV_D1;
			break;
		}
		case 1: {
			testPin = PIN_CV_D2;
			break;
		}
		case 2: {
			testPin = PIN_CV_D3;
			break;
		}
		case 3: {
			testPin = PIN_CV_D4;
			break;
		}
		case 4: {
			testPin = PIN_CV_MIN;
			break;
		}
		case 5: {
			testPin = PIN_CV_MAX;
			break;
		}
		default: {
			break;
		}
		}

	struct CVData cvData;

	analogWrite(testPin, 0);
	delay(500);
	getCV(&cvData);

	for (int cv=0; cv<6; cv++) { // check that all 6 CVs are set at 0.0V
		if ((cvData.volts[cv] < -0.1) || (cvData.volts[cv] > 0.1)) {
			sprintf(errorMsg, "SET CV 0.0v failed, expected all CVs at 0.0V, CV%i read at %f", cv+1, cvData.volts[cv]);
			return;
		}
	}

	//byte buff[DIAG_MAX_BYTES];
	//readModuleDiag(buff); ***** done in getCV

	analogWrite(testPin, 255/2); // Write 50% of maximum 3.3v, e.g. 1.65v
	delay(500); // PWM-to-analog converter takes a long time to settle on the target voltage
	getCV(&cvData);

	for (int cv=0; cv<6; cv++) { // check that all other CVs are set at 0.0V, CV under test at 1.65v
		if (cv != i) {
			if ((cvData.volts[cv] < -0.1) || (cvData.volts[cv] > 0.1)) {
				sprintf(errorMsg, "SET CV%i 1.65V failed, expected all other CVs at 0.0V, CV%i read at %f", i+1, cv+1, cvData.volts[cv]);
				return;
			}
		}
		else if ((cvData.volts[cv] < 1.55) || (cvData.volts[cv] > 1.75)) {
			sprintf(errorMsg, "SET CV 1.65V failed, CV%i read at %f", cv+1, cvData.volts[cv]);
			return;
		}
	}

	analogWrite(testPin, 255);
	delay(500);

	getCV(&cvData);

	for (int cv=0; cv<6; cv++) { // check that all other CVs are set at 0.0V, CV under test at 1.65v
		if (cv != i) {
			if ((cvData.volts[cv] < -0.1) || (cvData.volts[cv] > 0.1)) {
				sprintf(errorMsg, "SET CV 3.3V failed, expected all other CVs at 0.0V, CV%i read at %f", cv+1, cvData.volts[cv]);
				return;
			}
		}
		else if ((cvData.volts[cv] < 3.2) || (cvData.volts[cv] > 3.4)) {
			sprintf(errorMsg, "SET CV 3.3V failed, CV%i read at %f", cv+1, cvData.volts[cv]);
			return;
		}
	}

	// Read CV back from the module data stream

	//getCV("CV_MIN", &cvMin);

	// Return results
	//sendResponse("CV is: values(%i) percent(%f) volts(%f)", cvMin.value, cvMin.percent, cvMin.volts);
	analogWrite(testPin, 0); // Restore to 0.0v
}}

void verifyAudio() {

}

void verifyMemory() {

}
