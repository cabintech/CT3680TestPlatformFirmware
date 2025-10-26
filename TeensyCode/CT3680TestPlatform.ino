#include "Arduino.h"
#include <cstring>

#include <LiquidCrystal_I2C.h>

const int POWER_STATE_CHANGE_DELAY = 250;	// msec to wait after turn module on or off
const int LCD_INIT_DELAY			= 50;	// msec to wait on startup for LCD to be ready
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

	static constexpr int DIAG 		= 0;	// Module diagnostics data stream
	static constexpr int AUDIO_OUT 	= 21;	// Analog output
};

/**
 * Sets all pin modes and sets default output levels (except for I2C outputs
 * and the relay power control pin).
 */
void setDefaultConfig() {
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

	//pinMode(Pin::RELAY_POWER, OUTPUT);

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

	pinMode(Pin::DIAG, INPUT);			// Serial diagnostics datastream (no pullup/down)

	// Set default output levels (not including I2C or relay power pins)

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
	char buff[17] = "                ";
	int len = snprintf(buff, 17, "%s", str);
	if (len < 16) {
		buff[len] = " "; // Remove null so remainder of line is padded with blanks
	}
	lcd.setCursor(0, lineNum);
	lcd.print(buff);
}

/**
 * The setup function is called once at startup of the sketch
 *
 */
void setup()
{
	//pinMode(Pin::RELAY_POWER, OUTPUT);
	//digitalWrite(27, LOW);
	// Insure module power is off while we get initialized
	relayPower(false);

	// Init LCD display
	delay(LCD_INIT_DELAY);
	lcd.init();
	lcd.backlight();
	lcdPrintln(0, "Starting...");

	// Connect to USB serial port
	Serial.begin(9600);
	Serial.println("Program starting.");

	// Set all module controls to known default state
	setDefaultConfig();
	delay(100); // Settling time

	lcdPrintln(0, "Ready");

}

// The loop function is called in an endless loop
void loop()
{
	if (digitalRead(Pin::BUTTON_A)==LOW) {
		relayPower(true);
		lcdPrintln(0,  "Power ON");
		Serial.println("Button A down");
	}

	if (digitalRead(Pin::BUTTON_B)==LOW) {
		relayPower(false);
		lcdPrintln(0, "Power OFF");
		Serial.println("Button B down");
	}

	if (digitalRead(Pin::BUTTON_C)==LOW) {
		Serial.println("Button C down");
	}

	delay(1000);


}
