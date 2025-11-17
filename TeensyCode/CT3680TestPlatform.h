/*
 * CT3680TestPlatform.h
 *
 *  Created on: Nov 12, 2025
 *      Author: Editor
 */

#ifndef CT3680TESTPLATFORM_H_
#define CT3680TESTPLATFORM_H_

extern char errorMsg[200];

const int PIN_I2C_SCL = 19;	// Module programming
const int PIN_I2C_SDA = 18;

const int PIN_LCD_SDA = 17;	// LCD display (Wire1)
const int PIN_LCD_SCL = 16;

const int PIN_IV_A = 15;	// Module current sensor
const int PIN_IV_B = 14;

const int PIN_PGM0 = 32;	// Module program select
const int PIN_PGM1 = 31;
const int PIN_PGM2 = 30;
const int PIN_PGM3 = 10;

const int PIN_TT_IN = 40;	// Tap tempo

const int PIN_OPTION1 = 35;	// Option flags
const int PIN_OPTION2 = 36;
const int PIN_OPTION3 = 37;
const int PIN_OPTION4 = 38;
const int PIN_OPTION5 = 39;

const int PIN_RATE0 = 33;	// Sampling rate
const int PIN_RATE1 = 34;

const int PIN_RESET = 41;	// Module reset

const int PIN_CV_D1 = 6;// Digital PMW to analog VC delay controls
const int PIN_CV_D2 = 7;
const int PIN_CV_D3 = 8;
const int PIN_CV_D4 = 9;
const int PIN_CV_MAX = 24;
const int PIN_CV_MIN = 25;

const int PIN_RELAY_POWER = 27;	// Module power

const int PIN_SRC_SEL0 = 4;	// Source select (analog mux)
const int PIN_SRC_SEL1 = 5;

const int PIN_AIN_SEL0 = 2;	// Input channel select (analog mux)
const int PIN_AIN_SEL1 = 3;

const int PIN_AOUT_SEL0 = 23;// Output channel select (analog mux)
const int PIN_AOUT_SEL1 = 22;

const int PIN_AUDIO_IN = 12;	// Audio input source #3

const int PIN_BUTTON_A = 29;	// Buttons
const int PIN_BUTTON_B = 28;
const int PIN_BUTTON_C = 26;

//const int PIN_DIAG 		= 0;	// Serial1 Module diagnostics data stream
const int PIN_AUDIO_OUT = 21;	// Analog output



const int DIAG_MAX_BYTES = 150;		// Max size byte buffer required to read a diagnostics frame, including header


const int sampleRates[] = { 12000, 24000, 32000, 48000 };

const int DIAG_CV_MIN = 28;
const int DIAG_CV_MAX = 32;
const int DIAG_CV_1 = 36;
const int DIAG_CV_2 = 40;
const int DIAG_CV_3 = 44;
const int DIAG_CV_4 = 48;

const int CT3680_I2C_ADDR = 0x30; // FXCore I2C address
const int DIAG_STREAM_RATE = 4000;// Module serial diagnostics data rate (bps). 10 bits/byte, 400 bytes/sec, 1 byte every 2.5ms
const int DEBOUNCE_TIME = 25;	// msec to wait for key contacts to stabilize

const int LCD_INIT_DELAY = 50;	// msec to wait on startup for LCD to be ready


#endif /* CT3680TESTPLATFORM_H_ */
