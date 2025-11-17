#include "Arduino.h"
#include <stdio.h>
#include <stdarg.h>
#include "CT3680TestPlatform.h"
#include "util.h"
#include "Wire.h"

const int POWER_STATE_CHANGE_DELAY = 250;// msec to wait after turn module on or off
const byte FRAME_PREAMBLE = 0x7E;
const byte FRAME_POSTAMBLE = 0x6E;

/**
 * Turn on the module power, including required LOW->HIGH RESET transition.
 */
void relayPowerOn() {
	// Hold RESET low while power comes up
	digitalWrite(PIN_RESET, LOW);

	digitalWrite(PIN_RELAY_POWER, LOW); // Turn ON power relay
	delay(POWER_STATE_CHANGE_DELAY); // Give time for relay to switch and power to settle

	// RESET HIGH to enter FXCore run mode
	digitalWrite(PIN_RESET, HIGH);

	// FXCore requires time to fully startup
	delay(POWER_STATE_CHANGE_DELAY);
}

/**
 * Turn the module power off
 */
void relayPowerOff() {
	// Hold RESET LOW to halt the FXCore while I/O becomes unstable
	digitalWrite(PIN_RESET, LOW);
	delay(10);

	// Set all inputs to the module LOW so when power to the modules
	// goes off there is no power backflowing from the I/O pins
	digitalWrite(PIN_RATE0, LOW);
	digitalWrite(PIN_RATE1, LOW);
	digitalWrite(PIN_PGM0, LOW);
	digitalWrite(PIN_PGM1, LOW);
	digitalWrite(PIN_PGM2, LOW);
	digitalWrite(PIN_PGM3, LOW);
	digitalWrite(PIN_TT_IN, LOW);
	digitalWrite(PIN_OPTION1, LOW);
	digitalWrite(PIN_OPTION2, LOW);
	digitalWrite(PIN_OPTION3, LOW);
	digitalWrite(PIN_OPTION4, LOW);
	digitalWrite(PIN_OPTION5, LOW);
	digitalWrite(PIN_CV_D1, LOW);
	digitalWrite(PIN_CV_D2, LOW);
	digitalWrite(PIN_CV_D3, LOW);
	digitalWrite(PIN_CV_D4, LOW);
	digitalWrite(PIN_CV_MAX, LOW);
	digitalWrite(PIN_CV_MIN, LOW);
	digitalWrite(PIN_AUDIO_IN, LOW);	// Generate no audio on input source 3
	delay(10);

	digitalWrite(PIN_RELAY_POWER, HIGH); // Relay off
	delay(POWER_STATE_CHANGE_DELAY); // Give some time for relay to switch and power to settle
}

/**
 * Turn the power relay on (true) or off (false) to control power to the module. This
 * routine will add some delay for the relay switch time and power supply stabilization.
 */
void relayPower(bool turnOn) {
	if (turnOn) {
		relayPowerOn();
	}
	else {
		relayPowerOff();
	}
}


/**
 * Turns power onto the module with default pin configurations, then
 * measures the power (current) consumption. An error is set of the current
 * is above or below the expect range.
 */
void powerOn() {

	defaultModuleConfiguration(); // Inital state for module after power on

	//Turn on power relay
	relayPower(true);
	//verify current level

	// Determine voltage drop across SHUNT resistor (3 ohms)
	float voltageDif = calculateVoltage(analogRead(15))	- calculateVoltage(analogRead(14));

	// Expected voltage is ~0.3v as measured through 60% voltage dividers on either
	// side of the SHUNT. Allow +/- 15% variance.

	if (voltageDif > (0.3 * 1.15)) { // 0.345 volts
		relayPower(false); //turn module power off
		sprintf(errorMsg, "CURRENT TOO HIGH: voltageDif: %f", voltageDif); // Set errorMsg
		//	resetState();
		return;
	} else if (voltageDif < (0.3 * 0.85)) { // 0.255 volts
		relayPower(false); //turn module power off
		sprintf(errorMsg, "CURRENT TOO LOW: voltageDif: %f", voltageDif); // Set errorMsg
		//	resetState();
		return;
	}
	//verify 3.3v supply voltage
	//resetState();
}

/**
 * Power off the module (this has no failure modes)
 */
void powerOff() {
	relayPowerOff();
}

/**
 * Configure module inputs to defaults
 * 48kHz sampling
 * Program 15
 * Options1-5 HIGH
 * CV inputs zero
 * Select source 1
 * Select input channel 1
 * Select output channel 1
 */
void defaultModuleConfiguration() {
	digitalWrite(PIN_RATE0, HIGH);		// Set module to 48kHz sampling rate (requires reset to take effect)
	digitalWrite(PIN_RATE1, HIGH);
	digitalWrite(PIN_RESET, HIGH);		// Keep high until reset

	digitalWrite(PIN_PGM0, HIGH);		// Select program 15 (diagnostics pgm)
	digitalWrite(PIN_PGM1, HIGH);
	digitalWrite(PIN_PGM2, HIGH);
	digitalWrite(PIN_PGM3, HIGH);

	digitalWrite(PIN_TT_IN, HIGH);		// No tap tempo

	digitalWrite(PIN_OPTION1, HIGH);	// Default OPTION settings
	digitalWrite(PIN_OPTION2, HIGH);
	digitalWrite(PIN_OPTION3, HIGH);
	digitalWrite(PIN_OPTION4, HIGH);
	digitalWrite(PIN_OPTION5, HIGH);

	digitalWrite(PIN_CV_D1, LOW);		// Analog controls to zero
	digitalWrite(PIN_CV_D2, LOW);
	digitalWrite(PIN_CV_D3, LOW);
	digitalWrite(PIN_CV_D4, LOW);
	digitalWrite(PIN_CV_MAX, LOW);
	digitalWrite(PIN_CV_MIN, LOW);

	digitalWrite(PIN_SRC_SEL0, LOW);	// Select input source 1
	digitalWrite(PIN_SRC_SEL1, LOW);
	digitalWrite(PIN_AIN_SEL0, LOW);	// Connect source to input channel 1
	digitalWrite(PIN_AIN_SEL1, LOW);
	digitalWrite(PIN_AOUT_SEL0, LOW);	// Select output from channel 1
	digitalWrite(PIN_AOUT_SEL1, LOW);

	digitalWrite(PIN_AUDIO_IN, LOW);	// Generate no audio on input source 3
}

//calculates the voltage associated the teensy's returned integer from 0-1023
float calculateVoltage(int reading) {
	float voltage = (reading / 1023.0) * 3.3;
	return voltage;
}

/**
 * Sets all pin modes
 */
void setPinModes() {
	// Set pin modes

	//pinMode(PIN_I2C_SCL, OUTPUT); // I2C interfaces
	//pinMode(PIN_I2C_SDA,OUTPUT);
	//pinMode(PIN_LCD_SCL, OUTPUT);
	//pinMode(PIN_LCD_SDA,OUTPUT);

	pinMode(PIN_IV_A, INPUT);	// Current sensor inputs (no pullup or pulldown)
	pinMode(PIN_IV_B, INPUT);

	pinMode(PIN_RATE0, OUTPUT);		// Sampling rate controls
	pinMode(PIN_RATE1, OUTPUT);
	pinMode(PIN_RESET, OUTPUT);

	pinMode(PIN_PGM0, OUTPUT);		// Program select
	pinMode(PIN_PGM1, OUTPUT);
	pinMode(PIN_PGM2, OUTPUT);
	pinMode(PIN_PGM3, OUTPUT);

	pinMode(PIN_TT_IN, OUTPUT);	// Tap tempo module input

	pinMode(PIN_OPTION1, OUTPUT);	// Option pins
	pinMode(PIN_OPTION2, OUTPUT);
	pinMode(PIN_OPTION3, OUTPUT);
	pinMode(PIN_OPTION4, OUTPUT);
	pinMode(PIN_OPTION5, OUTPUT);

	pinMode(PIN_CV_D1, OUTPUT);	// PWM (analog) output delay controls
	pinMode(PIN_CV_D2, OUTPUT);
	pinMode(PIN_CV_D3, OUTPUT);
	pinMode(PIN_CV_D4, OUTPUT);
	pinMode(PIN_CV_MAX, OUTPUT);
	pinMode(PIN_CV_MIN, OUTPUT);

	pinMode(PIN_RELAY_POWER, OUTPUT);

	pinMode(PIN_SRC_SEL0, OUTPUT);		// Source select
	pinMode(PIN_SRC_SEL1, OUTPUT);
	pinMode(PIN_AIN_SEL0, OUTPUT);		// Input channel select
	pinMode(PIN_AIN_SEL1, OUTPUT);
	pinMode(PIN_AOUT_SEL0, OUTPUT);	// Output channel select
	pinMode(PIN_AOUT_SEL1, OUTPUT);

	pinMode(PIN_AUDIO_IN, OUTPUT);		// Generated audio (input source 3)
	pinMode(PIN_AUDIO_OUT, INPUT);		// Audio output (via AOUT select)

	pinMode(PIN_BUTTON_A, INPUT_PULLUP);// Buttons, read HIGH unless button connects input to GND
	pinMode(PIN_BUTTON_B, INPUT_PULLUP);
	pinMode(PIN_BUTTON_C, INPUT_PULLUP);

}

int reverseBytes(const byte *buff) {
	return (buff[3] << 24) + (buff[2] << 16) + (buff[1] << 8) + buff[0];
}

float convertIntervalToMS(int interval, int rate) {
	int sampleRate = sampleRates[rate];
	return interval * (1000.0 / sampleRate);
}

//check for error messages
bool checkForErrors() {
	if (errorMsg[0] != 0) {
		return true;
	} else {
		return false;
	}
}

/**
 * Sends a command response to the host via the Serial port. If an error
 * message is current pending in the global errorMsg then it is sent
 * prefixed with "ERROR: " as the response. Otherwise the supplied message
 * (with optional sprintf() substitutions) is sent prefixed with "OK: ". Example:
 *
 * int i = getSomeValue();
 * sendResponse("OK: Value was %i", i);
 */
void sendResponse(const char * response, ...) {
	if (checkForErrors()) {
		Serial.print("ERROR: ");
		Serial.println(errorMsg);
		//reset for future error messages
		errorMsg[0] = 0;
	} else {
	    char buffer[256]; 	// A buffer to store the formatted string
	    va_list args;		// Variable arg list (see

	    va_start(args, response); // Initialize va_list with the last fixed argument
	    vsnprintf(buffer, sizeof(buffer), response, args); // Use vsnprintf for safety
	    va_end(args); // Clean up va_list
	    Serial.print("OK: ");
		Serial.println(buffer);
	}
}

void clearModuleConfiguration() {
	// Set all inputs pins of the module to LOW so there are no transient voltages as
	// the modules is inserted or removed from the fixture.

	digitalWrite(PIN_RATE0, LOW);
	digitalWrite(PIN_RATE1, LOW);
	digitalWrite(PIN_RESET, LOW);
	digitalWrite(PIN_PGM0, LOW);
	digitalWrite(PIN_PGM1, LOW);
	digitalWrite(PIN_PGM2, LOW);
	digitalWrite(PIN_PGM3, LOW);

	digitalWrite(PIN_TT_IN, LOW);

	digitalWrite(PIN_OPTION1, LOW);
	digitalWrite(PIN_OPTION2, LOW);
	digitalWrite(PIN_OPTION3, LOW);
	digitalWrite(PIN_OPTION4, LOW);
	digitalWrite(PIN_OPTION5, LOW);

	digitalWrite(PIN_CV_D1, LOW);
	digitalWrite(PIN_CV_D2, LOW);
	digitalWrite(PIN_CV_D3, LOW);
	digitalWrite(PIN_CV_D4, LOW);
	digitalWrite(PIN_CV_MAX, LOW);
	digitalWrite(PIN_CV_MIN, LOW);

	digitalWrite(PIN_SRC_SEL0, LOW);
	digitalWrite(PIN_SRC_SEL1, LOW);
	digitalWrite(PIN_AIN_SEL0, LOW);
	digitalWrite(PIN_AIN_SEL1, LOW);
	digitalWrite(PIN_AOUT_SEL0, LOW);
	digitalWrite(PIN_AOUT_SEL1, LOW);

	digitalWrite(PIN_AUDIO_IN, LOW);
}

/**
 * Use the FXCore's I2C interface to query it's serial number. The SN is
 * returned as a 10 byte null terminated string into the supplied buffer,
 * in the form "0Xffffffff". The buffer must be at least 11 bytes long.
 */
void getModuleSN(char *buff) {

	// Send cmd to enter programming mode (see FXCore docs)
	Wire.beginTransmission(CT3680_I2C_ADDR);
	Wire.write(0xA5);
	Wire.write(0x5A);
	Wire.write(CT3680_I2C_ADDR);
	Wire.endTransmission();

	// Request FXCore status data (12 bytes)
	Wire.requestFrom(CT3680_I2C_ADDR, 12);
	int status[12];
	for (int i = 0; i < 12; i++) {
		if (Wire.available()) {
			status[i] = Wire.read();
		} else {
			status[i] = 0;
		}
	}

	// Format results into caller-supplied buffer
	snprintf(buff, 11, "0X%02x%02x%02x%02x", status[11], status[10], status[9],
			status[8]);

	// Return FXCore to RUN mode
	Wire.beginTransmission(0x30);
	Wire.write(0x5A);
	Wire.write(0xA5);
	Wire.endTransmission();

}

/**
 * Returns the current diagnostics datastream values for the
 * given VC name ("VC_xxxx"). The caller must supply a pointer to
 * a struct of type CVData. This function will write the current
 * values to the supplied struct. Example:
 *
 * struct VCData myCV;
 * getCV("CV_DELAY1", &myCV);
 * if (myCV.volts < 1.6) {
 * ...
 */
void fillInCVData(struct CVData *cvData, int cvVal, int ind) {
	cvData->values[ind] = cvVal; // FXCore int value from 0 to 0x7FFFFFFF
	//cvData->percent[ind] = (float)cvData->values / 0x7FFFFFFF; // Ratio (percentage) of maximum value
	//cvData->volts[ind] = cvData->percent * 3.3; // Voltage in 0.0 to 3.3 range
}

void getCV(struct CVData *cvData) {

	byte diag[DIAG_MAX_BYTES];
	readModuleDiag(diag);
	if (checkForErrors()) return;

	int cv1 = reverseBytes(diag+DIAG_CV_1);
	int cv2 = reverseBytes(diag+DIAG_CV_2);
	int cv3 = reverseBytes(diag+DIAG_CV_3);
	int cv4 = reverseBytes(diag+DIAG_CV_4);
	int max = reverseBytes(diag+DIAG_CV_MAX);
	int min = reverseBytes(diag+DIAG_CV_MIN);

	fillInCVData(cvData, cv1, 0);
	fillInCVData(cvData, cv2, 1);
	fillInCVData(cvData, cv3, 2);
	fillInCVData(cvData, cv4, 3);
	fillInCVData(cvData, max, 4);
	fillInCVData(cvData, min, 5);

	return;
}


//read the next frame in the diagnostic stream
void readModuleDiag(byte *buff) {
	// Read diagnostics data from the module's serial output port.
	//Serial.println("Start diagnostic read");
	Serial1.begin(DIAG_STREAM_RATE); // Default is 8 data bits, 0 parity bits, 1 stop bit

	// Crummy buffer design in Arduino... if 64 byte serial buffer is full,
	// incoming data is discarded. So buffer contains oldest data seen since the
	// last read (not newest). So to get current datastream we must clear the
	// buffer, which depends on being able to read it faster than than the sender
	// (CT3680) is sending bytes. Since the baud rate is only 4000
	// bits/sec, a simple read loop will do, but not really a great design (the
	// Serial interface should have a clear() buffer method).
	while (Serial1.available()) {
		Serial1.read();
		//Serial.println("Discarded old data");
	}

	// Now we start looking frame-start preamble 0x7E7E7E7E (this code is similar to Mon3680.java)
	int delayCounter = 0;
	int startCounter = 0;
	int singleBuffer = 0;
	while (startCounter < 4) {
		// Wait for next byte to arrive
		while (!Serial1.available()) {
			delay(2); // Rate is 2.5ms per byte, so don't wait more than that
			delayCounter++;
			if (delayCounter > 500) {
				sprintf(errorMsg, "No DIAG preamble found in 1 second.");
				return;
			}
		}

		singleBuffer = Serial1.read();
		if (singleBuffer == FRAME_PREAMBLE) {
			startCounter++;
			//Serial.println("Found 7E byte");
		} else {
			startCounter = 0; // Bytes must be sequential, any non-7E means start over
			//Serial.print("Found non-7E byte: x");
			//Serial.println(singleBuffer, 16);
		}
	}

	// Frame preamble was found, 8 byte header is expected next

	int headerLen = Serial1.readBytes(buff, 8);
	if (headerLen != 8) {
		sprintf(errorMsg,
				"Expected 8 header bytes in diagnostic frame, received %d",
				headerLen);
		return;
	}

	int contentLength = buff[0]; // Extract content length (# of 32-bit words) from the header
	contentLength = contentLength * 4; // Calc number of content bytes

	int bytesRead = Serial1.readBytes(buff + 8, contentLength); // Read the frame content
	//Serial.print("Content len = ");
	//Serial.println(bytesRead);

	if (bytesRead != contentLength) {
		sprintf(errorMsg, "Expected %d bytes in diagnostic frame, received %d",
				contentLength, bytesRead);
		return;
	}

	// Read 4 byte frame ending sequence
	byte postAmble[4];
	int postLen = Serial1.readBytes(postAmble, 4);
	if (postLen != 4) {
		sprintf(errorMsg, "Postamble read only %i bytes, expected 4", postLen);
	}
	else if (postAmble[0]!=FRAME_POSTAMBLE || postAmble[1]!=FRAME_POSTAMBLE || postAmble[2]!=FRAME_POSTAMBLE || postAmble[3]!=FRAME_POSTAMBLE) {
		sprintf(errorMsg, "Postamble contained invalid bytes.");
	}

}
