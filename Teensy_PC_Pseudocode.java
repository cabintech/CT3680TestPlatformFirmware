//PC
//
//Define constants for commands: POWER_ON, POWER_OFF, SET_CV, etc
//Connect to serial port
//While more commands
//  Write to serial port
//  Wait for response
//  Process response (OK, FAIL, etc.)


//uC
//
//Define constants for commands
//Connect to serial port in setUp()
//In loop()
//  switch(command):
//      A:
//          powerOn()
//      B:
//          powerOff()
//      C:
//          setCV(val)
//      ...