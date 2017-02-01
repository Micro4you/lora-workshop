# LoRa Workshop

# Requirements

* [Arduino IDE](https://www.arduino.cc/en/main/software)
* [Teensyduino Arduino add-on](https://www.pjrc.com/teensy/td_download.html)
* [The Things Network account](https://www.thethingsnetwork.org/)
* [Node-RED](https://nodered.org/docs/getting-started/installation)
* [relayr account](https://developer.relayr.io/)

# Repository

This git repository contains the following three code snippets:

* `usb-to-serial` Arduino code, which enables you to configure the RN2483 LoRa module by using the serial terminal on your computer.
* `sensor-node` Arduino code, which reads the sensor readings and sends them every 30 seconds over pre-configured RN2483 LoRa module on your *The Things Network account*.
* `node-red` snippet, which subscribes to The Things Network MQTT stream and forwards it to your *relayr Dashboard*

## Configuring the RN2483 module

1. Compile and upload the `usb-to-serial` arduino code on Teensy LC.
2. Open the Serial Monitor in Arduino IDE (set the right port in Tools tab and `Both NL & CR` as the new line character). Now you can interact with RN2483 module through its [command / response interface](http://ww1.microchip.com/downloads/en/DeviceDoc/40001784B.pdf).
3. The next step is to setup the neccessary LoRaWAN keys and saving them to EEPROM (`mac save`). For *activation by personalization (abp)* use the following commands:

```
mac set devaddr <device address>
mac set nwkskey <network session key>
mac set appskey <application session key>
mac save
```

You can generate you `device address`, `network session key` and `application session key` by creating an application and a device on your The Things Network account.

Alternatively you could use *over the air activation* instead of *abp*:

```
mac set appeui <appEUI>
mac set appkey <app key>
mac save
mac join otaa
```

### Test

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