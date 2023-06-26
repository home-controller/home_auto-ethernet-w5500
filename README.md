# Home Automation Arduino Ethernet Relay/Lights Controller
This project aims to control lights using a wired Ethernet connection with the W5500 or W5100 module, instead of relying on Wi-Fi. Other Ethernet modules can be used with different libraries.

This Arduino-based relay controller supports ATmega328P chips (Nano, Micro, UNO, etc.) using the W5500 or W5100 Ethernet modules.

The controller features a web server on port 80 and an MQTT client to toggle relays, enabling control over outside lights in me case but could be anyting that requires a relay etc. to turn it on and off.

Switches can be connected to the Arduino pins for controlling the relays via MQTT or the web server on port 80.# Home Auto Arduino ethernet relay/lights controller

## MQTT examples
To view MQTT messages on the server, run the following command:
```
mosquitto_sub -v -t '#' -F '%I %t %p'
```
* -v: Enables verbose output, displaying the topic and message.
* -t '#': Subscribes to all topics.
* -F '%I %t %p': Formats the output to display the client IP address (%I), topic (%t), and message payload (%p).

### to control a light

To turn off a light:
```
mosquitto_pub -h localhost -t h1/c1/outside/s/e -m 0
```

To turn on a light:
```
mosquitto_pub -h localhost -t h1/c1/outside/s/e -m 0
```

The controller board will publish the following messages when it successfully switches the light on/off:
```
state/h1/c1/outside/s/e s=0
state/h1/c1/outside/s/e s=1
```
etc. when it susesflly switches the light on/off
The state/ is added to the topic to show that the device is updade and so it don't get confused with the a command to control the board.
