#include "Arduino.h"
#include <cstring>

#include <LiquidCrystal_I2C.h>

#include "CT3680TestPlatform.h"
#include "util.h"
#include "verify.h"

char errorMsg[200];

const char *POWER_ON = "POWER_ON";
const char *POWER_OFF = "POWER_OFF";
const char *READ_SN = "READ_SN";
const char *TEST_DIAG = "TEST_DIAG";
const char *TEST_OPT = "TEST_OPT";
const char *TEST_PGM = "TEST_PGM";
const char *TEST_SR = "TEST_SR";
const char *TEST_TT = "TEST_TT";
const char *TEST_CV = "TEST_CV";
const char *TEST_AUD = "TEST_AUD";
const char *TEST_MEM = "TEST_MEM";

LiquidCrystal_I2C lcd(0x27, 20, 2);	// Configure LCD at I2S address 0x27, 16 chars, 2 lines
//TODO: Should that be "16" instead of "20"??

/**
 * Print a line of text to the LCD display (max 16 chars). This will replace the entire line.
 */
void lcdPrintln(int lineNum, char const *str) {
	char buff[20];
	snprintf(buff, 20, "%-16s", str); // Pad with blanks to 16 chars
	lcd.setCursor(0, lineNum);
	lcd.print(buff);
}

/**
 * Returns when the given button is released plus a small de-bounce time.
 */
void waitForButtonRelease(int pinNum) {
	while (true) {
		if (digitalRead(pinNum) == HIGH) { // Released
			delay(DEBOUNCE_TIME);
			if (digitalRead(pinNum) == HIGH) { // Still released
				return;
			}
		}
		delay(250); // Check every 1/4 second
	}
}

/**
 * Wait for any button to be pressed and return the button
 * 'A', 'B', or 'C'.
 */
char waitForAnyButton(const char *prompt1, const char *prompt2) {
	if (prompt1 != NULL) {
		lcdPrintln(0, prompt1);
	}
	if (prompt2 != NULL) {
		lcdPrintln(1, prompt2);
	}

	while (true) {
		if (digitalRead(PIN_BUTTON_A) == LOW) {
			waitForButtonRelease(PIN_BUTTON_A);
			return 'A';
		}

		if (digitalRead(PIN_BUTTON_B) == LOW) {
			waitForButtonRelease(PIN_BUTTON_B);
			return 'B';
		}

		if (digitalRead(PIN_BUTTON_C) == LOW) {
			waitForButtonRelease(PIN_BUTTON_C);
			return 'C';
		}
		delay(250);
	}
}

/**
 * The setup function is called once at startup of the sketch
 *
 */
void setup() {
	errorMsg[0] = 0;
	setPinModes();
	digitalWrite(PIN_RELAY_POWER, HIGH);
	powerOff();

//	// Init LCD display
//	delay(LCD_INIT_DELAY);
//	lcd.init();
//	lcd.backlight();
//	lcdPrintln(0, "Starting...");
//
//	// Connect to USB serial port
	Serial.begin(9600);
//	Serial.println("Program starting.");

//
//	// Setup FXCore I2C interface
//	Wire.begin(); // We are I2C controller of this bus
//
//	delay(100); // Settling time
//
//	lcdPrintln(0, "Ready");


}

// The loop function is called in an endless loop
void loop() {
//	// Test button press code
//	//char b = waitForAnyButton("Btn test", "A      B       C");
//	//char buff[40];
//	//sprintf(buff, "Button %c pressed", b);
//	//Serial.println(buff);
//
//	// Prep for module insert
//	relayPower(false);
//	clearModuleConfiguration();
//
//	// Power is OFF, module inputs are LOW. Safe to insert now.
//	char c = waitForAnyButton("Insert Module","OK            NO");
//
//	if (c == 'A') {
//		defaultModuleConfiguration();
//		relayPower(true);
//		char b1 = ' ';
//		while (b1 != 'A') {
//			b1 = waitForAnyButton("","OFF    TEST");
//
//			if (b1 == 'B') {
//				// TEST: Get module SN via FXCore I2C interface
//				//char sn[11];
//				//getModuleSN(sn);
//				//Serial.print("Module SN: ");
//				//Serial.println(sn);
//
//				// TEST: Read module diagnostics data stream
//				readModuleDiag();
//
//			}
//		}
//	}

	char buffer[50];

	if (Serial.available()) {
		int bytesRead = Serial.readBytesUntil('\n', buffer, 49);
		buffer[bytesRead] = '\0';  // Null-terminate the string

		//Serial.println("You said: ");
		//Serial.println(buffer);

		if (strcmp(buffer, POWER_ON) == 0) {
			powerOn();
			sendResponse("Power is ON");
		} else if (strcmp(buffer, POWER_OFF) == 0) {
			powerOff();
			sendResponse("Power is OFF");
		} else if (strcmp(buffer, READ_SN) == 0) {
			char snBuffer[20];
			getModuleSN(snBuffer);
			sendResponse(snBuffer);
		} else if (strcmp(buffer, TEST_DIAG) == 0) {
			verifyDiagStream();
			sendResponse("Diagnostic frame detected.");
		} else if (strcmp(buffer, TEST_OPT) == 0) {
			verifyOptionSwitches();
			sendResponse("All options verified");
		} else if (strcmp(buffer, TEST_PGM) == 0) {
			verifyPGMSwitches();
			sendResponse("All programs verified");
		} else if (strcmp(buffer, TEST_SR) == 0) {
			verifySamplingRates();
			sendResponse("All sampling rates verified");
		} else if (strcmp(buffer, TEST_TT) == 0) {
			verifyTapTempo();
			sendResponse("Tap tempo verified");
		} else if (strcmp(buffer, TEST_CV) == 0) {
			verifyVCSettings();
			sendResponse("All CVs verified");
		} else if (strcmp(buffer, TEST_AUD) == 0) {
			verifyAudio();
			sendResponse("Audio passthrough verified");
		} else if (strcmp(buffer, TEST_MEM) == 0) {
			verifyMemory();
			sendResponse("Delay line verified");
		} else {
			sprintf(errorMsg, "Invalid command '%s'", buffer);
			sendResponse("Invalid command");
		}
	}

	delay(100);

//	int len = Serial.readBytesUntil('\n', buffer, 50);
//	String * commandParts = strtok(command, ":");
//	char * commandMain = commandParts[0];
//
//	if (strcmp(commandMain, POWER_ON) == 0) {
//		pwrTest;
//	}
//	else if (strcmp(commandMain, POWER_OFF) == 0) {
//		relayPower(false);
//	}
}
