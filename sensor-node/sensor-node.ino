/*
 * sensor-node.ino
 * 
 * This sketch joins the LoRaWAN network using the provided keys and sends
 * data from two sensors: A magnetic switch and an infrared proximity sensor.
 * 
 * Copyright (c) 2017 relayr GmbH
 * Website    : https://relayr.io
 * Author     : Klemen Lilija (klemen@relayr.io)
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

// Set to 0 to deactivate the debugging messages over the serial port
#define DEBUG 1

// Pin definitions
const int proximity_sensor_pin = 0;
const int reed_switch_pin = 15;
const int vcc_rn2483 = 2;
const int led = 13;

// RN2483 sleeping mode flag
bool sleeping = false;



//------------------------------------------------------------------------------------//
// Setup function: Configuration and initialization                                   //
//------------------------------------------------------------------------------------//

void setup() {
  // String in which we will feed serial output from the RN2483
  String rn2483_response = "";
  
  // Debugging interface
  #if DEBUG
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for the serial port to connect
  }
  #endif
  
  // Intitalize the UART interface with the RN2483
  Serial1.begin(57600);
  
  // Initialize the pins
  pinMode(reed_switch_pin, INPUT_PULLDOWN);
  pinMode(vcc_rn2483, OUTPUT);
  pinMode(led, OUTPUT);
  
  // Turn the RN2483 on
  digitalWrite(vcc_rn2483, HIGH);
  
  // Reset the RN2483 and wait for response
  Serial1.println("sys reset");
  Serial1.flush();
  rn2483_response = Serial1.readString();
  
  // Join the network with Activation By Personalization (ABP)
  Serial1.println("mac join abp");
  Serial1.flush();
  rn2483_response += Serial1.readString();
  
  // Debugging messages
  #if DEBUG
  Serial.println(rn2483_response);
  #endif
  
  // Send the first readings
  sendData(getSensorHexReadings());
}



//------------------------------------------------------------------------------------//
// This is the MAIN LOOP, it's repeated until the end of time! :)                     //
//------------------------------------------------------------------------------------//

void loop() {
  if (Serial1.available() && sleeping) {
    
    // Set the sleeping flag to false.
    sleeping = false;
    
    // Read the serial buffer
    String rn2483_response = Serial1.readString();
    sendData(getSensorHexReadings());

    // Debugging messages
    #if DEBUG
    Serial.println(rn2483_response);
    #endif
  }
}



//------------------------------------------------------------------------------------//
// Return sensor readings in a hex string                                             //
// (1st byte: proximity, 2nd byte: reed switch)                                       //
//------------------------------------------------------------------------------------//

String getSensorHexReadings(){
  int proximity = getProximity();
  bool switch_state = getSwitchState();
  return (String(proximity, HEX) + "0" + String(switch_state, HEX));
}



//------------------------------------------------------------------------------------//
// Send data using the RN2483 LoRaWAN module                                          //
//------------------------------------------------------------------------------------//

void sendData(String hexData){
  // String in which we will feed serial output from the RN2483
  String rn2483_response = "";
  
  // Send data
  Serial1.println("mac tx uncnf 3 " + hexData);
  Serial1.flush();
  rn2483_response = Serial1.readString();
  
  // Wait for the tx response
  while(!Serial1.available()){
  }
  rn2483_response += Serial1.readString();
  
  // Blink an LED
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  
  // Sleep for 30 seconds
  Serial1.println("sys sleep 30000");
  Serial1.flush();
  
  // Set the sleeping flag
  sleeping = true;
  
  // Debugging messages
  #if DEBUG
  Serial.println("Sending data: " + hexData);
  Serial.println(rn2483_response);
  #endif
}



//------------------------------------------------------------------------------------//
// Return IR proximitiy sensor reading (cm)                                          //
//------------------------------------------------------------------------------------//

int getProximity() {
  int sum = 0;
  int sensor_value;
  int proximity;

  // Average the ADC readings (n=20)
  for (int i=0;i<20;i++){
    sensor_value = analogRead(proximity_sensor_pin);
    sum += sensor_value;
  }
  
  sensor_value = sum/20;
  // Map the ADC readings to an aproximate range of the sensor
  proximity = constrain(map(sensor_value, 731, 180, 10, 80),10,80);
  return proximity;
}


//------------------------------------------------------------------------------------//
// Return reed switch state                                                           //
//------------------------------------------------------------------------------------//

bool getSwitchState() {
  bool state;
  state = digitalRead(reed_switch_pin);
  return state;
}

