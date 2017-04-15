/*
 * marvin-usb-to-serial.ino
 * 
 * This sketch is specific for the Marvin board:
 * https://www.kickstarter.com/projects/688158475/marvin-the-lora-development-board
 * 
 * It establishes a serial communication over the UART interface between the
 * ATmega32u4 and the RN2483 module, so it can be configured with the LoRaWan keys.
 * This code is a 1:1 port of the original code "usb-to-serial.ino" for the
 * Marvin board (http://kck.st/2fdp9aa). Only the pins have been changed to
 * comply with the layout of the board and the microcontroller.
 * 
 * To flash the Marvin board, on the Arduino IDE, head to "Tools" -> "Boards",
 * and select Arduino Leonardo instead.
 * 
 * Copyright (c) 2017 relayr GmbH
 * Website    : https://relayr.io
 * Author     : Klemen Lilija (klemen@relayr.io)
 * Adapted by : Jaime González-Arintero (jaime@relayr.io)
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */



// In the Marvin board, the RN2483 is powered via the pin 5
const int vcc_rn2483 = 5;



//------------------------------------------------------------------------------------//
// Setup function: Configuration and initialization                                   //
//------------------------------------------------------------------------------------//

void setup(){
 // Initialize the serial port 
  Serial.begin(9600);
  
  while (!Serial) {
    // Just wait for the serial port to connect
    // This is required for native USB port only
    ;
  }

  // Initialize the UART (serial) interface with the RN2483
  Serial1.begin(57600);
  pinMode(vcc_rn2483, OUTPUT);
  digitalWrite(vcc_rn2483, HIGH);
}



//------------------------------------------------------------------------------------//
// This is the MAIN LOOP, it's repeated until the end of time! :)                     //
//------------------------------------------------------------------------------------//

void loop() {
  // Forward messages from one serial interface to another, and the other way around
  // We can communicate with the ATmega32u4 and this, in turn, with the RN2483 module
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
