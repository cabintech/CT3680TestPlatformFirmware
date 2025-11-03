#include "Arduino.h"
#include <cstring>

#include <LiquidCrystal_I2C.h>

const int CT3680_I2C_ADDR			= 0x30; // FXCore I2C address
const int DIAG_STREAM_RATE			= 4000;	// Module serial diagnostics data rate (bps). 10 bits/byte, 400 bytes/sec, 1 byte every 2.5ms
const int DEBOUNCE_TIME 			= 25;	// msec to wait for key contacts to stabilize
const int POWER_STATE_CHANGE_DELAY = 250;	// msec to wait after turn module on or off
const int LCD_INIT_DELAY			= 50;	// msec to wait on startup for LCD to be ready
const char * POWER_ON = "POWER_ON";
const char * POWER_OFF = "POWER_OFF";
LiquidCrystal_I2C lcd(0x27, 20, 2);			// Configure LCD at I2S address 0x27, 16 chars, 2 lines
//TODO: Should that be "16" instead of "20"??

struct Pin {
 	static constexpr int I2C_SCL 	= 19;	// Module programming
	static constexpr int I2C_SDA 	= 18;

	static constexpr int LCD_SDA 	= 17;	// LCD display (Wire1)
	static constexpr int LCD_SCL 	= 16;

	static constexpr int IV_A 		= 15;	// Module current sensor
	static constexpr int IV_B 		= 14;

	static constexpr int PGM0 		= 32;	// Module program select
	static constexpr int PGM1 		= 31;
	static constexpr int PGM2 		= 30;
	static constexpr int PGM3 		= 10;

	static constexpr int TT_IN 		= 40;	// Tap tempo

	static constexpr int OPTION1 	= 35;	// Option flags
	static constexpr int OPTION2 	= 36;
	static constexpr int OPTION3 	= 37;
	static constexpr int OPTION4 	= 38;
	static constexpr int OPTION5 	= 39;

	static constexpr int RATE0 		= 33;	// Sampling rate
	static constexpr int RATE1 		= 34;

	static constexpr int RESET 		= 41;	// Module reset

	static constexpr int DVC_D1 	= 6;	// Digital PMW to analog VC delay controls
	static constexpr int DVC_D2 	= 7;
	static constexpr int DVC_D3 	= 8;
	static constexpr int DVC_D4 	= 9;
	static constexpr int DVC_MAX 	= 24;
	static constexpr int DVC_MIN 	= 25;

	static constexpr int RELAY_POWER= 27;	// Module power

	static constexpr int SRC_SEL0 	= 4;	// Source select (analog mux)
	static constexpr int SRC_SEL1 	= 5;

	static constexpr int AIN_SEL0 	= 2;	// Input channel select (analog mux)
	static constexpr int AIN_SEL1 	= 3;

	static constexpr int AOUT_SEL0 	= 23;	// Output channel select (analog mux)
	static constexpr int AOUT_SEL1 	= 22;

	static constexpr int AUDIO_IN 	= 12;	// Audio input source #3

	static constexpr int BUTTON_A 	= 29;	// Buttons
	static constexpr int BUTTON_B 	= 28;
	static constexpr int BUTTON_C 	= 26;

	//static constexpr int DIAG 		= 0;	// Serial1 Module diagnostics data stream
	static constexpr int AUDIO_OUT 	= 21;	// Analog output
};

/**
 * Sets all pin modes
 */
void setPinModes() {
	// Set pin modes

	//pinMode(Pin::I2C_SCL, OUTPUT); // I2C interfaces
	//pinMode(Pin::I2C_SDA,OUTPUT);
	//pinMode(Pin::LCD_SCL, OUTPUT);
	//pinMode(Pin::LCD_SDA,OUTPUT);

	pinMode(Pin::IV_A, INPUT);		// Current sensor inputs (no pullup or pulldown)
	pinMode(Pin::IV_B, INPUT);

	pinMode(Pin::RATE0,OUTPUT);		// Sampling rate controls
	pinMode(Pin::RATE1,OUTPUT);
	pinMode(Pin::RESET,OUTPUT);

	pinMode(Pin::PGM0,OUTPUT);		// Program select
	pinMode(Pin::PGM1,OUTPUT);
	pinMode(Pin::PGM2,OUTPUT);
	pinMode(Pin::PGM3,OUTPUT);

	pinMode(Pin::TT_IN, OUTPUT);	// Tap tempo module input

	pinMode(Pin::OPTION1,OUTPUT);	// Option pins
	pinMode(Pin::OPTION2,OUTPUT);
	pinMode(Pin::OPTION3,OUTPUT);
	pinMode(Pin::OPTION4,OUTPUT);
	pinMode(Pin::OPTION5,OUTPUT);

	pinMode(Pin::DVC_D1,OUTPUT);	// PWM (analog) output delay controls
	pinMode(Pin::DVC_D2,OUTPUT);
	pinMode(Pin::DVC_D3,OUTPUT);
	pinMode(Pin::DVC_D4,OUTPUT);
	pinMode(Pin::DVC_MAX,OUTPUT);
	pinMode(Pin::DVC_MIN,OUTPUT);

	pinMode(Pin::RELAY_POWER, OUTPUT);

	pinMode(Pin::SRC_SEL0, OUTPUT);		// Source select
	pinMode(Pin::SRC_SEL1, OUTPUT);
	pinMode(Pin::AIN_SEL0, OUTPUT);		// Input channel select
	pinMode(Pin::AIN_SEL1, OUTPUT);
	pinMode(Pin::AOUT_SEL0, OUTPUT);	// Output channel select
	pinMode(Pin::AOUT_SEL1, OUTPUT);

	pinMode(Pin::AUDIO_IN, OUTPUT);		// Generated audio (input source 3)
	pinMode(Pin::AUDIO_OUT, INPUT);		// Audio output (via AOUT select)

	pinMode(Pin::BUTTON_A, INPUT_PULLUP);// Buttons, read HIGH unless button connects input to GND
	pinMode(Pin::BUTTON_B, INPUT_PULLUP);
	pinMode(Pin::BUTTON_C, INPUT_PULLUP);


}

/**
 * Turn the power relay on (true) or off (false) to control power to the module. This
 * routine will add some delay for the relay switch time and power supply stabilization.
 */
void relayPower(bool turnOn) {
	digitalWrite(Pin::RELAY_POWER, !turnOn);
	delay(POWER_STATE_CHANGE_DELAY); // Give time for relay to switch and power to settle
}

/**
 * Print a line of text to the LCD display (max 16 chars). This will replace the entire line.
 */
void lcdPrintln(int lineNum, char const * str) {
	char buff[20];
	snprintf(buff, 20, "%-16s", str); // Pad with blanks to 16 chars
	lcd.setCursor(0, lineNum);
	lcd.print(buff);
}

void clearModuleConfiguration() {
	// Set all inputs pins of the module to LOW so there are no transient voltages as
	// the modules is inserted or removed from the fixture.

	digitalWrite(Pin::RATE0, LOW);
	digitalWrite(Pin::RATE1, LOW);
	digitalWrite(Pin::RESET, LOW);
	digitalWrite(Pin::PGM0, LOW);
	digitalWrite(Pin::PGM1, LOW);
	digitalWrite(Pin::PGM2, LOW);
	digitalWrite(Pin::PGM3, LOW);

	digitalWrite(Pin::TT_IN, LOW);

	digitalWrite(Pin::OPTION1, LOW);
	digitalWrite(Pin::OPTION2, LOW);
	digitalWrite(Pin::OPTION3, LOW);
	digitalWrite(Pin::OPTION4, LOW);
	digitalWrite(Pin::OPTION5, LOW);

	digitalWrite(Pin::DVC_D1, LOW);
	digitalWrite(Pin::DVC_D2, LOW);
	digitalWrite(Pin::DVC_D3, LOW);
	digitalWrite(Pin::DVC_D4, LOW);
	digitalWrite(Pin::DVC_MAX, LOW);
	digitalWrite(Pin::DVC_MIN, LOW);

	digitalWrite(Pin::SRC_SEL0, LOW);
	digitalWrite(Pin::SRC_SEL1, LOW);
	digitalWrite(Pin::AIN_SEL0, LOW);
	digitalWrite(Pin::AIN_SEL1, LOW);
	digitalWrite(Pin::AOUT_SEL0, LOW);
	digitalWrite(Pin::AOUT_SEL1, LOW);

	digitalWrite(Pin::AUDIO_IN, LOW);
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
	digitalWrite(Pin::RATE0, HIGH);		// Set module to 48kHz sampling rate (requires reset to take effect)
	digitalWrite(Pin::RATE1, HIGH);
	digitalWrite(Pin::RESET, HIGH);		// Keep high until reset

	digitalWrite(Pin::PGM0, HIGH);		// Select program 15 (diagnostics pgm)
	digitalWrite(Pin::PGM1, HIGH);
	digitalWrite(Pin::PGM2, HIGH);
	digitalWrite(Pin::PGM3, HIGH);

	digitalWrite(Pin::TT_IN, HIGH);		// No tap tempo

	digitalWrite(Pin::OPTION1, HIGH);	// Default OPTION settings
	digitalWrite(Pin::OPTION2, HIGH);
	digitalWrite(Pin::OPTION3, HIGH);
	digitalWrite(Pin::OPTION4, HIGH);
	digitalWrite(Pin::OPTION5, HIGH);

	digitalWrite(Pin::DVC_D1, LOW);		// Analog controls to zero
	digitalWrite(Pin::DVC_D2, LOW);
	digitalWrite(Pin::DVC_D3, LOW);
	digitalWrite(Pin::DVC_D4, LOW);
	digitalWrite(Pin::DVC_MAX, LOW);
	digitalWrite(Pin::DVC_MIN, LOW);

	digitalWrite(Pin::SRC_SEL0, LOW);	// Select input source 1
	digitalWrite(Pin::SRC_SEL1, LOW);
	digitalWrite(Pin::AIN_SEL0, LOW);	// Connect source to input channel 1
	digitalWrite(Pin::AIN_SEL1, LOW);
	digitalWrite(Pin::AOUT_SEL0, LOW);	// Select output from channel 1
	digitalWrite(Pin::AOUT_SEL1, LOW);

	digitalWrite(Pin::AUDIO_IN, LOW);	// Generate no audio on input source 3
}

/**
 * The setup function is called once at startup of the sketch
 *
 */
void setup()
{
	// Establish all the I/O pin modes
	setPinModes();

	// Ensure module power is off while we get initialized
	relayPower(false);

//	// Init LCD display
//	delay(LCD_INIT_DELAY);
//	lcd.init();
//	lcd.backlight();
//	lcdPrintln(0, "Starting...");
//
//	// Connect to USB serial port
	Serial.begin(9600);
	Serial.println("Program starting.");

//
//	// Setup FXCore I2C interface
//	Wire.begin(); // We are I2C controller of this bus
//
//	delay(100); // Settling time
//
//	lcdPrintln(0, "Ready");

}

/**
 * Returns when the given button is released plus a small de-bounce time.
 */
void waitForButtonRelease(int pinNum) {
	while (true) {
		if (digitalRead(pinNum)==HIGH) { // Released
			delay(DEBOUNCE_TIME);
			if (digitalRead(pinNum)==HIGH) { // Still released
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
char waitForAnyButton(const char * prompt1, const char * prompt2) {
	if (prompt1 != NULL) {
		lcdPrintln(0, prompt1);
	}
	if (prompt2 != NULL) {
		lcdPrintln(1, prompt2);
	}

	while (true) {
		if (digitalRead(Pin::BUTTON_A)==LOW) {
			waitForButtonRelease(Pin::BUTTON_A);
			return 'A';
		}

		if (digitalRead(Pin::BUTTON_B)==LOW) {
			waitForButtonRelease(Pin::BUTTON_B);
			return 'B';
		}

		if (digitalRead(Pin::BUTTON_C)==LOW) {
			waitForButtonRelease(Pin::BUTTON_C);
			return 'C';
		}
		delay(250);
	}
}

/**
 * Use the FXCore's I2C interface to query it's serial number. The SN is
 * returned as a 10 byte null terminated string into the supplied buffer,
 * in the form "0Xffffffff". The buffer must be at least 11 bytes long.
 */
void getModuleSN(char * buff) {

	// Send cmd to enter programming mode (see FXCore docs)
	Wire.beginTransmission(CT3680_I2C_ADDR);
	Wire.write(0xA5);
	Wire.write(0x5A);
	Wire.write(CT3680_I2C_ADDR);
	Wire.endTransmission();

	// Request FXCore status data (12 bytes)
	Wire.requestFrom(CT3680_I2C_ADDR, 12);
	int status[12];
	for (int i=0; i<12; i++) {
		if (Wire.available()) {
			status[i] = Wire.read();
		}
		else {
			status[i] = 0;
		}
	}

	// Format results into caller-supplied buffer
	snprintf(buff, 11, "0X%02x%02x%02x%02x", status[11], status[10], status[9], status[8]);

	// Return FXCore to RUN mode
	Wire.beginTransmission(0x30);
	Wire.write(0x5A);
	Wire.write(0xA5);
	Wire.endTransmission();

}

void readModuleDiag() {
	// Read diagnostics data from the module's serial output port.
	Serial.println("Start diag read");
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
				Serial.println("No DIAG header found in 1 second.");
				return;
			}
		}

		singleBuffer = Serial1.read();
		if (singleBuffer==0x7E) {
			startCounter++;
			//Serial.println("Found 7E byte");
		}
		else {
			startCounter = 0; // Bytes must be sequential, any non-7E means start over
			//Serial.print("Found non-7E byte: x");
			//Serial.println(singleBuffer, 16);
		}
	}

	Serial.println("DIAG frame preamble received.");

	byte header[8];
	int headerLen = Serial1.readBytes(header, 8);
	if (headerLen != 8) {
		Serial.print("ERROR: Expected 8 header bytes in diag frame, received ");
		Serial.println(headerLen);
		return;
	}

	Serial.print("Frame data length = ");Serial.println(header[0]);


}

// The loop function is called in an endless loop
void loop()
{
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


	    Serial.print("You said: ");
	    Serial.println(buffer);

	    	if (strcmp(buffer, POWER_ON) == 0) {
	    		relayPower(true);
	    	}
	    	else if (strcmp(buffer, POWER_OFF) == 0) {
	    		relayPower(false);
	    	}
	    	//will add more commands here for each of the testing procedures
	  }


//	int len = Serial.readBytesUntil('\n', buffer, 50);
//	String * commandParts = strtok(command, ":");
//	char * commandMain = commandParts[0];
//
//	if (strcmp(commandMain, POWER_ON) == 0) {
//		relayPower(true);
//	}
//	else if (strcmp(commandMain, POWER_OFF) == 0) {
//		relayPower(false);
//	}
}
