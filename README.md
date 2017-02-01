# LoRa Workshop

## Requirements

* [Arduino IDE](https://www.arduino.cc/en/main/software)
* [Teensyduino Arduino add-on](https://www.pjrc.com/teensy/td_download.html)
* [The Things Network account](https://www.thethingsnetwork.org/)
* [Node-RED](https://nodered.org/docs/getting-started/installation)
* [relayr account](https://developer.relayr.io/)

## Repository

This git repository contains the following three code snippets:

* `usb-to-serial` Arduino code, which enables you to configure the RN2483 LoRa module by using the serial terminal on your computer.
* `sensor-node` Arduino code, which reads the sensor readings and sends them every 30 seconds over pre-configured RN2483 LoRa module on your *The Things Network account*.
* `node-red` snippet, which subscribes to The Things Network MQTT stream and forwards it to your *relayr Dashboard*

## Interfacing with the Teensy LC

1. On the Arduino IDE, click on `File` → `Open`, and look for the Arduino sketch `usb-to-serial.ino` that can be found in the folder with the same name.

2. Now click on `Tools` → `Board`, and select `Teensy LC`, as shown below.  
![](./assets/arduino_ide_mac_board_selection.png)

3. Your Teensy LC board should be connected to your computer via USB. Select the right port clicking on `Tools` → `Port`. An example of this step in OSX is depicted for clarity.  
![](./assets/arduino_ide_mac_port_selection.png)

4. Open the Serial Monitor in the Arduino IDE. Simply click on the small icon of a magnifying glass in the top right corner.  
![](./assets/arduino_ide_serial_monitor_button.png)

5. The Serial Monitor window will open, displaying a message from the RN2483 module. The Teensy LC is now acting as an interface between our computer and such module.   
![](./assets/arduino_ide_serial_monitor_window.png)

6. Instead of `No line ending`, select `Both NL & CR` as the new line character.  
![](./assets/arduino_ide_serial_monitor_NL_CR.png)

Now we are ready to interact with the RN2483 module through its [command / response interface](http://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf), so we proceed to the next part where it will be configured.

## Configuring the RN2483 Module

On this part we will be setting the necessary LoRaWAN keys and saving them to the module's EEPROM. With these keys our device can be activated (and thus, recognized by the gateway). Since there are two ways of performing this activation, both methods are covered below.

### Activation By Personalization (ABP)

On this part we will be setting the necessary LoRaWAN keys and saving them to EEPROM. The three  (`mac save`). For *activation by personalization (abp)* use the following commands:

```
mac set devaddr <device address>
mac set nwkskey <network session key>
mac set appskey <application session key>
mac save
```

You can generate you `device address`, `network session key` and `application session key` by creating an application and a device on your The Things Network account.

### Over The Air Activation (OTAA)

Alternatively you could use *over the air activation* instead of *abp*:

```
mac set appeui <appEUI>
mac set appkey <app key>
mac save
mac join otaa
```

## Test

To test if everything is working fine you can try to join the network and manually send some data:

Join the network:

```
mac join <mode>
<mode>: abp (activation by personalization),
        otaa (over the air activation)

e.g. mac join abp
```

Send data:

```
mac tx <type> <portno> <data>
<type>: cnf (confirmable)
        uncnf (unconfirmable)
<portno>: number between 1 and 223
<data>: hex payload

e.g. mac tx uncnf 4 3A1B
```

You should see the data appear in your `Application Data` on The Things Network.

## TheThingsNetwork to relayr Platform

Now you can use `node-red` to subscribe to data recived by the things network and forward it to your account on relayr to store and visulize the data.

## References

### RN2483

[Command Reference](http://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf)

### TTN

[Create an account](https://account.thethingsnetwork.org/register)

### Node-red

[Getting Started Guide](https://nodered.org/docs/getting-started/)

[TTN nodes](https://www.npmjs.com/package/node-red-contrib-ttn)

### relayr

[Developer Dashboard](https://developer.relayr.io/)

## License

Copyright (C) 2017 relayr GmbH, Klemen Lilija <klemen@relayr.io>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

Except as contained in this notice, the name(s) of the above copyright holders shall not be used in advertising or otherwise to promote the sale, use or other dealings in this Software without prior written authorization.

THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.